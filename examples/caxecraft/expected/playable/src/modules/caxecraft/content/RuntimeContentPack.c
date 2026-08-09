#include "hxc/program.h"

void hxc_compiler_constructor_caxecraft_content_RuntimeContentRegistry(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_packId, int32_t hxc_l_packVersion, hxc_string hxc_l_logicalPath, hxc_string hxc_l_airBlock, hxc_string hxc_l_defaultAquaticProfileId, struct hxc_array_ref *hxc_l_features, struct hxc_array_ref *hxc_l_blocks, struct hxc_array_ref *hxc_l_fluids, struct hxc_array_ref *hxc_l_aquaticProfiles, struct hxc_array_ref *hxc_l_items, struct hxc_array_ref *hxc_l_npcs, struct hxc_array_ref *hxc_l_enemies, struct hxc_array_ref *hxc_l_drops, struct hxc_array_ref *hxc_l_effects)
{
  const void *hxc_l_gc_roots[10] = { (const void *)hxc_l_self, (const void *)hxc_l_features, (const void *)hxc_l_blocks, (const void *)hxc_l_fluids, (const void *)hxc_l_aquaticProfiles, (const void *)hxc_l_items, (const void *)hxc_l_npcs, (const void *)hxc_l_enemies, (const void *)hxc_l_drops, (const void *)hxc_l_effects };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 10, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_TField_packIdValue_managed_String_initializer_owner_n15 = hxc_l_packId;
  if (hxc_string_retain(hxc_l_tmp_TField_packIdValue_managed_String_initializer_owner_n15) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_packIdValue = hxc_l_tmp_TField_packIdValue_managed_String_initializer_owner_n15;
  (*hxc_l_self).hxc_packVersionValue = hxc_l_packVersion;
  hxc_string hxc_l_tmp_TField_logicalPathValue_managed_String_initializer_owner_n16 = hxc_l_logicalPath;
  if (hxc_string_retain(hxc_l_tmp_TField_logicalPathValue_managed_String_initializer_owner_n16) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_logicalPathValue = hxc_l_tmp_TField_logicalPathValue_managed_String_initializer_owner_n16;
  hxc_string hxc_l_tmp_TField_airBlock_managed_String_initializer_owner_n17 = hxc_l_airBlock;
  if (hxc_string_retain(hxc_l_tmp_TField_airBlock_managed_String_initializer_owner_n17) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_airBlock = hxc_l_tmp_TField_airBlock_managed_String_initializer_owner_n17;
  hxc_string hxc_l_tmp_TField_defaultAquaticProfileId_managed_String_initializer_owner_n18 = hxc_l_defaultAquaticProfileId;
  if (hxc_string_retain(hxc_l_tmp_TField_defaultAquaticProfileId_managed_String_initializer_owner_n18) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_defaultAquaticProfileId = hxc_l_tmp_TField_defaultAquaticProfileId_managed_String_initializer_owner_n18;
  (*hxc_l_self).hxc_features = hxc_l_features;
  (*hxc_l_self).hxc_blocks = hxc_l_blocks;
  (*hxc_l_self).hxc_fluids = hxc_l_fluids;
  (*hxc_l_self).hxc_aquaticProfiles = hxc_l_aquaticProfiles;
  (*hxc_l_self).hxc_items = hxc_l_items;
  (*hxc_l_self).hxc_npcs = hxc_l_npcs;
  (*hxc_l_self).hxc_enemies = hxc_l_enemies;
  (*hxc_l_self).hxc_drops = hxc_l_drops;
  (*hxc_l_self).hxc_effects = hxc_l_effects;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_compiler_constructor_caxecraft_content_RuntimePresentation(struct hxc_caxecraft_content_RuntimePresentation *hxc_l_self, hxc_string hxc_l_asset, hxc_string hxc_l_cell, int32_t hxc_l_cellIndex)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_TField_asset_managed_String_initializer_owner_n4 = hxc_l_asset;
  if (hxc_string_retain(hxc_l_tmp_TField_asset_managed_String_initializer_owner_n4) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_asset = hxc_l_tmp_TField_asset_managed_String_initializer_owner_n4;
  hxc_string hxc_l_tmp_TField_cell_managed_String_initializer_owner_n5 = hxc_l_cell;
  if (hxc_string_retain(hxc_l_tmp_TField_cell_managed_String_initializer_owner_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_cell = hxc_l_tmp_TField_cell_managed_String_initializer_owner_n5;
  (*hxc_l_self).hxc_cellIndex = hxc_l_cellIndex;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition *hxc_l_self, hxc_string hxc_l_id, int32_t hxc_l_line, int32_t hxc_l_column, int32_t hxc_l_maximumBreathTicks, int32_t hxc_l_breathRecoveryPerTick, int32_t hxc_l_horizontalControlMilli, int32_t hxc_l_ascentAccelerationMilli, int32_t hxc_l_descentAccelerationMilli, int32_t hxc_l_buoyancyAccelerationMilli, int32_t hxc_l_dragPerTickMilli, int32_t hxc_l_drowningIntervalTicks, bool hxc_l_underwaterMining, bool hxc_l_coldProtection)
{
  const void *hxc_l_gc_roots[2] = { (const void *)hxc_l_self, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 2, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[1] = (const void *)&hxc_l_self->hxc_base;
  hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeLocatedId(&hxc_l_self->hxc_base, hxc_l_id, hxc_l_line, hxc_l_column);
  (*hxc_l_self).hxc_maximumBreathTicks = hxc_l_maximumBreathTicks;
  (*hxc_l_self).hxc_breathRecoveryPerTick = hxc_l_breathRecoveryPerTick;
  (*hxc_l_self).hxc_horizontalControlMilli = hxc_l_horizontalControlMilli;
  (*hxc_l_self).hxc_ascentAccelerationMilli = hxc_l_ascentAccelerationMilli;
  (*hxc_l_self).hxc_descentAccelerationMilli = hxc_l_descentAccelerationMilli;
  (*hxc_l_self).hxc_buoyancyAccelerationMilli = hxc_l_buoyancyAccelerationMilli;
  (*hxc_l_self).hxc_dragPerTickMilli = hxc_l_dragPerTickMilli;
  (*hxc_l_self).hxc_drowningIntervalTicks = hxc_l_drowningIntervalTicks;
  (*hxc_l_self).hxc_underwaterMining = hxc_l_underwaterMining;
  (*hxc_l_self).hxc_coldProtection = hxc_l_coldProtection;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_self, hxc_string hxc_l_id, int32_t hxc_l_line, int32_t hxc_l_column, int32_t hxc_l_storageCode, bool hxc_l_passable, bool hxc_l_collectable, struct hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference hxc_l_dropItem, hxc_string hxc_l_renderProfile)
{
  const void *hxc_l_gc_roots[3] = { (const void *)hxc_l_self, hxc_l_dropItem.hxc_tag == hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady ? (const void *)hxc_l_dropItem.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference : NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 3, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[2] = (const void *)&hxc_l_self->hxc_base;
  hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeLocatedId(&hxc_l_self->hxc_base, hxc_l_id, hxc_l_line, hxc_l_column);
  (*hxc_l_self).hxc_storageCode = hxc_l_storageCode;
  (*hxc_l_self).hxc_passable = hxc_l_passable;
  (*hxc_l_self).hxc_collectable = hxc_l_collectable;
  (*hxc_l_self).hxc_dropItem = hxc_l_dropItem;
  hxc_string hxc_l_tmp_TField_renderProfile_managed_String_initializer_owner_n9 = hxc_l_renderProfile;
  if (hxc_string_retain(hxc_l_tmp_TField_renderProfile_managed_String_initializer_owner_n9) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_renderProfile = hxc_l_tmp_TField_renderProfile_managed_String_initializer_owner_n9;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeDropDefinition(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeDropDefinition *hxc_l_self, hxc_string hxc_l_id, int32_t hxc_l_line, int32_t hxc_l_column, struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_item, int32_t hxc_l_quantity, int32_t hxc_l_pickupRadiusMilli, struct hxc_caxecraft_content_RuntimePresentation *hxc_l_presentation)
{
  const void *hxc_l_gc_roots[4] = { (const void *)hxc_l_self, (const void *)hxc_l_item, (const void *)hxc_l_presentation, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 4, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[3] = (const void *)&hxc_l_self->hxc_base;
  hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeLocatedId(&hxc_l_self->hxc_base, hxc_l_id, hxc_l_line, hxc_l_column);
  (*hxc_l_self).hxc_item = hxc_l_item;
  (*hxc_l_self).hxc_quantity = hxc_l_quantity;
  (*hxc_l_self).hxc_pickupRadiusMilli = hxc_l_pickupRadiusMilli;
  (*hxc_l_self).hxc_presentation = hxc_l_presentation;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeEffectDefinition(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEffectDefinition *hxc_l_self, hxc_string hxc_l_id, int32_t hxc_l_line, int32_t hxc_l_column)
{
  const void *hxc_l_gc_roots[2] = { (const void *)hxc_l_self, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 2, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[1] = (const void *)&hxc_l_self->hxc_base;
  hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeLocatedId(&hxc_l_self->hxc_base, hxc_l_id, hxc_l_line, hxc_l_column);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_self, hxc_string hxc_l_id, int32_t hxc_l_line, int32_t hxc_l_column, int32_t hxc_l_maximumHealth, int32_t hxc_l_noticeRadiusMilli, int32_t hxc_l_strikeRadiusMilli, int32_t hxc_l_attackRadiusMilli, int32_t hxc_l_windupTicks, int32_t hxc_l_recoveryTicks, int32_t hxc_l_stepMilli, struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_drop, struct hxc_caxecraft_content_RuntimePresentation *hxc_l_presentation)
{
  const void *hxc_l_gc_roots[4] = { (const void *)hxc_l_self, (const void *)hxc_l_drop, (const void *)hxc_l_presentation, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 4, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[3] = (const void *)&hxc_l_self->hxc_base;
  hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeLocatedId(&hxc_l_self->hxc_base, hxc_l_id, hxc_l_line, hxc_l_column);
  (*hxc_l_self).hxc_maximumHealth = hxc_l_maximumHealth;
  (*hxc_l_self).hxc_noticeRadiusMilli = hxc_l_noticeRadiusMilli;
  (*hxc_l_self).hxc_strikeRadiusMilli = hxc_l_strikeRadiusMilli;
  (*hxc_l_self).hxc_attackRadiusMilli = hxc_l_attackRadiusMilli;
  (*hxc_l_self).hxc_windupTicks = hxc_l_windupTicks;
  (*hxc_l_self).hxc_recoveryTicks = hxc_l_recoveryTicks;
  (*hxc_l_self).hxc_stepMilli = hxc_l_stepMilli;
  (*hxc_l_self).hxc_drop = hxc_l_drop;
  (*hxc_l_self).hxc_presentation = hxc_l_presentation;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeFluidDefinition(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeFluidDefinition *hxc_l_self, hxc_string hxc_l_id, int32_t hxc_l_line, int32_t hxc_l_column, struct hxc_caxecraft_content_RuntimePresentation *hxc_l_presentation)
{
  const void *hxc_l_gc_roots[3] = { (const void *)hxc_l_self, (const void *)hxc_l_presentation, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 3, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[2] = (const void *)&hxc_l_self->hxc_base;
  hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeLocatedId(&hxc_l_self->hxc_base, hxc_l_id, hxc_l_line, hxc_l_column);
  (*hxc_l_self).hxc_presentation = hxc_l_presentation;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_self, hxc_string hxc_l_id, int32_t hxc_l_line, int32_t hxc_l_column, int32_t hxc_l_maximumStack, enum hxc_caxecraft_content_RuntimeItemUseProfile hxc_l_useProfile, struct hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference hxc_l_placementBlock, struct hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference hxc_l_aquaticProfile, struct hxc_caxecraft_content_RuntimePresentation *hxc_l_presentation)
{
  const void *hxc_l_gc_roots[5] = { (const void *)hxc_l_self, hxc_l_placementBlock.hxc_tag == hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady ? (const void *)hxc_l_placementBlock.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference : NULL, hxc_l_aquaticProfile.hxc_tag == hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady ? (const void *)hxc_l_aquaticProfile.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference : NULL, (const void *)hxc_l_presentation, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 5, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[4] = (const void *)&hxc_l_self->hxc_base;
  hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeLocatedId(&hxc_l_self->hxc_base, hxc_l_id, hxc_l_line, hxc_l_column);
  (*hxc_l_self).hxc_maximumStack = hxc_l_maximumStack;
  (*hxc_l_self).hxc_useProfile = hxc_l_useProfile;
  (*hxc_l_self).hxc_placementBlock = hxc_l_placementBlock;
  (*hxc_l_self).hxc_aquaticProfile = hxc_l_aquaticProfile;
  (*hxc_l_self).hxc_presentation = hxc_l_presentation;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeKindId(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeKindId *hxc_l_self, hxc_string hxc_l_id, hxc_string hxc_l_kind)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_TField_id_managed_String_initializer_owner_n3 = hxc_l_id;
  if (hxc_string_retain(hxc_l_tmp_TField_id_managed_String_initializer_owner_n3) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_id = hxc_l_tmp_TField_id_managed_String_initializer_owner_n3;
  hxc_string hxc_l_tmp_TField_kind_managed_String_initializer_owner_n4 = hxc_l_kind;
  if (hxc_string_retain(hxc_l_tmp_TField_kind_managed_String_initializer_owner_n4) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_kind = hxc_l_tmp_TField_kind_managed_String_initializer_owner_n4;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeLocatedId(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeLocatedId *hxc_l_self, hxc_string hxc_l_id, int32_t hxc_l_line, int32_t hxc_l_column)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_TField_id_managed_String_initializer_owner_n4 = hxc_l_id;
  if (hxc_string_retain(hxc_l_tmp_TField_id_managed_String_initializer_owner_n4) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_id = hxc_l_tmp_TField_id_managed_String_initializer_owner_n4;
  (*hxc_l_self).hxc_line = hxc_l_line;
  (*hxc_l_self).hxc_column = hxc_l_column;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeNpcDefinition(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeNpcDefinition *hxc_l_self, hxc_string hxc_l_id, int32_t hxc_l_line, int32_t hxc_l_column, int32_t hxc_l_maximumHealth, int32_t hxc_l_interactionRadiusMilli, struct hxc_caxecraft_content_RuntimePresentation *hxc_l_presentation)
{
  const void *hxc_l_gc_roots[3] = { (const void *)hxc_l_self, (const void *)hxc_l_presentation, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 3, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[2] = (const void *)&hxc_l_self->hxc_base;
  hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeLocatedId(&hxc_l_self->hxc_base, hxc_l_id, hxc_l_line, hxc_l_column);
  (*hxc_l_self).hxc_maximumHealth = hxc_l_maximumHealth;
  (*hxc_l_self).hxc_interactionRadiusMilli = hxc_l_interactionRadiusMilli;
  (*hxc_l_self).hxc_presentation = hxc_l_presentation;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeReference(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_self, hxc_string hxc_l_id, int32_t hxc_l_line, int32_t hxc_l_column)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_TField_id_managed_String_initializer_owner_n4 = hxc_l_id;
  if (hxc_string_retain(hxc_l_tmp_TField_id_managed_String_initializer_owner_n4) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_id = hxc_l_tmp_TField_id_managed_String_initializer_owner_n4;
  (*hxc_l_self).hxc_line = hxc_l_line;
  (*hxc_l_self).hxc_column = hxc_l_column;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

bool hxc_caxecraft_content_RuntimeContentPack_addKind(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, struct hxc_array_ref *hxc_l_kinds, struct hxc_caxecraft_content_RuntimeContentPack_RuntimeLocatedId *hxc_l_entry, hxc_string hxc_l_kind)
{
  const void *hxc_l_gc_roots[6] = { (const void *)hxc_l_kinds, (const void *)hxc_l_entry, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 6, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_enum_payload_0_owner_n6 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_2_owner_n7 = { 0 };
  int32_t hxc_l_g = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n0 = hxc_l_g;
    int32_t hxc_l_tmp_array_length_result_n1;
    if (hxc_array_ref_length(hxc_l_kinds, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n0 < hxc_l_tmp_array_length_result_n1))
    {
      break;
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeKindId *hxc_l_tmp_array_get_result_n3;
    if (hxc_array_ref_get_copy(hxc_l_kinds, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_array_get_result_n3;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeKindId *hxc_l_existing = hxc_l_tmp_array_get_result_n3;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeKindId *hxc_l_tmp_load_result_n5 = hxc_l_existing;
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_load_result_n5;
    if (hxc_l_tmp_load_result_n5 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n6 = (*hxc_l_tmp_load_result_n5).hxc_id;
    if (hxc_l_entry == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n7 = (*hxc_l_entry).hxc_id;
    if (hxc_l_tmp_class_field_load_result_n6.data == NULL || hxc_l_tmp_class_field_load_result_n7.data == NULL ? hxc_l_tmp_class_field_load_result_n6.data == hxc_l_tmp_class_field_load_result_n7.data : hxc_l_tmp_class_field_load_result_n6.byte_length == hxc_l_tmp_class_field_load_result_n7.byte_length && (hxc_l_tmp_class_field_load_result_n6.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n6.data, hxc_l_tmp_class_field_load_result_n7.data, hxc_l_tmp_class_field_load_result_n6.byte_length) == 0))
    {
      if (hxc_l_reader == NULL)
      {
        abort();
      }
      int32_t hxc_l_tmp_class_field_load_result_n9 = (*hxc_l_entry).hxc_line;
      int32_t hxc_l_tmp_class_field_load_result_n10 = (*hxc_l_entry).hxc_column;
      hxc_string hxc_l_tmp_class_field_load_result_n11 = (*hxc_l_entry).hxc_id;
      hxc_l_tmp_enum_payload_0_owner_n6 = hxc_l_tmp_class_field_load_result_n11;
      if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n6) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_instance_call_argument_2_owner_n7 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaCrossKindId, .hxc_payload.hxc_SchemaCrossKindId.hxc_id = hxc_l_tmp_enum_payload_0_owner_n6 };
      hxc_caxecraft_content_RuntimeSchemaReader_rejectAt(hxc_l_reader, hxc_l_tmp_class_field_load_result_n9, hxc_l_tmp_class_field_load_result_n10, hxc_l_tmp_instance_call_argument_2_owner_n7);
      hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_2_owner_n7);
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return false;
    }
  }
  if (hxc_l_entry == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_class_field_load_result_n15 = (*hxc_l_entry).hxc_id;
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeKindId *hxc_l_tmp_managed_class_result_n16 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_RuntimeContentPack_RuntimeKindId_descriptor, (void **)&hxc_l_tmp_managed_class_result_n16) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_managed_class_result_n16;
  hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeKindId(hxc_l_tmp_managed_class_result_n16, hxc_l_tmp_class_field_load_result_n15, hxc_l_kind);
  hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_managed_class_result_n16;
  int32_t hxc_l_tmp_array_push_result_n17;
  if (hxc_array_ref_push_copy(hxc_l_kinds, &(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeKindId *){ hxc_l_tmp_managed_class_result_n16 }, &hxc_l_tmp_array_push_result_n17) != HXC_STATUS_OK)
  {
    abort();
  }
  (void)hxc_l_tmp_array_push_result_n17;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return true;
}

struct hxc_caxecraft_content_RuntimeContentPackResult hxc_caxecraft_content_RuntimeContentPack_decode(struct hxc_bytes_ref *hxc_l_input, struct hxc_caxecraft_content_RuntimeAssetInventory *hxc_l_assets)
{
  const void *hxc_l_gc_roots[171] = { (const void *)hxc_l_assets, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 171, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_enum_payload_0_owner_n12 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n13 = { 0 };
  hxc_string hxc_l_packId = { 0 };
  hxc_string hxc_l_logicalPath = { 0 };
  hxc_string hxc_l_manifestId = { 0 };
  hxc_string hxc_l_air = { 0 };
  hxc_string hxc_l_defaultAquatic = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n23 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n24 = { 0 };
  hxc_string hxc_l_reserved_h8da772c5a155 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n37 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n38 = { 0 };
  hxc_string hxc_l_reserved_h16797aa67ceb = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n42 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n43 = { 0 };
  hxc_string hxc_l_reserved_h8fce842889d2 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n47 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n48 = { 0 };
  hxc_string hxc_l_reserved_h36c4a708a71d = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n52 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n53 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n55 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n56 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_instance_call_argument_2_owner_n6 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaReader hxc_l_tmp_object_storage_n3 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_class_object_address_n0 = &hxc_l_tmp_object_storage_n3;
  hxc_compiler_constructor_caxecraft_content_RuntimeSchemaReader(hxc_l_tmp_class_object_address_n0);
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader = hxc_l_tmp_class_object_address_n0;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n1 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n1 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n2 = hxc_caxecraft_content_RuntimeSchemaReader_parse(hxc_l_tmp_load_result_n1, hxc_l_input);
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_instance_call_result_n2;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_root = hxc_l_tmp_instance_call_result_n2;
  hxc_l_gc_roots[2] = (const void *)hxc_l_root;
  hxc_l_gc_roots[3] = (const void *)NULL;
  if (hxc_l_root == NULL)
  {
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n5 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[4] = hxc_l_tmp_call_result_n5.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n5.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n5;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n6 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n6 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n7 = hxc_l_root;
  hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_load_result_n7;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n8 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n8) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"schemaVersion", 13, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"logicalPath", 11, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"packId", 6, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"packVersion", 11, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"assetManifestId", 15, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"airBlock", 8, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"defaultAquaticProfile", 21, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"features", 8, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"blocks", 6, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"fluids", 6, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"aquaticProfiles", 15, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"items", 5, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"npcs", 4, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"enemies", 7, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"drops", 5, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"effects", 7, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"prefabs", 7, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"statefulObjects", 15, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"states", 6, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"signals", 7, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_instance_call_argument_2_owner_n6 = hxc_l_tmp_array_create_result_n8;
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n10 = hxc_caxecraft_content_RuntimeSchemaReader_object(hxc_l_tmp_load_result_n6, hxc_l_tmp_load_result_n7, (hxc_string){ (const uint8_t *)"content pack", 12, true, NULL }, hxc_l_tmp_instance_call_argument_2_owner_n6);
  hxc_l_gc_roots[6] = (const void *)hxc_l_tmp_instance_call_result_n10;
  struct hxc_array_ref *hxc_l_fields = hxc_l_tmp_instance_call_result_n10;
  hxc_l_gc_roots[7] = (const void *)hxc_l_fields;
  hxc_l_gc_roots[8] = (const void *)NULL;
  if (hxc_l_fields == NULL)
  {
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n13 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[9] = hxc_l_tmp_call_result_n13.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n13.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n13;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n14 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n14 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[10] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n16 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n14, hxc_l_fields, (hxc_string){ (const uint8_t *)"schemaVersion", 13, true, NULL });
  hxc_l_gc_roots[11] = (const void *)hxc_l_tmp_instance_call_result_n16;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_schemaNode = hxc_l_tmp_instance_call_result_n16;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n17 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n17 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[12] = (const void *)hxc_l_schemaNode;
  struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_instance_call_result_n19 = hxc_caxecraft_content_RuntimeSchemaReader_integer(hxc_l_tmp_load_result_n17, hxc_l_schemaNode, (hxc_string){ (const uint8_t *)"schemaVersion", 13, true, NULL }, 0, 2147483647);
  struct hxc_optional_value_h76f8e80e9d2b hxc_l_schemaVersion = hxc_l_tmp_instance_call_result_n19;
  if (!hxc_l_schemaVersion.hxc_has_value)
  {
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n22 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[13] = hxc_l_tmp_call_result_n22.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n22.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n22;
  }
  struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_optional_scalar_equality_optional_n9 = hxc_l_schemaVersion;
  int32_t hxc_l_tmp_optional_scalar_equality_scalar_n10 = 2;
  bool hxc_l_tmp_optional_scalar_equality_result_n11 = true;
  if (hxc_l_tmp_optional_scalar_equality_optional_n9.hxc_has_value)
  {
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_optional_scalar_compare_optional_load_result_n25 = hxc_l_tmp_optional_scalar_equality_optional_n9;
    int32_t hxc_l_tmp_optional_scalar_compare_scalar_load_result_n26 = hxc_l_tmp_optional_scalar_equality_scalar_n10;
    if (!hxc_l_tmp_optional_scalar_compare_optional_load_result_n25.hxc_has_value)
    {
      abort();
    }
    hxc_l_tmp_optional_scalar_equality_result_n11 = hxc_l_tmp_optional_scalar_compare_optional_load_result_n25.hxc_value != hxc_l_tmp_optional_scalar_compare_scalar_load_result_n26;
  }
  if (hxc_l_tmp_optional_scalar_equality_result_n11)
  {
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n28 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n28 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n29 = hxc_l_schemaNode;
    hxc_l_gc_roots[14] = (const void *)hxc_l_tmp_load_result_n29;
    hxc_l_tmp_enum_payload_0_owner_n12 = (hxc_string){ (const uint8_t *)"schemaVersion", 13, true, NULL };
    if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n12) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n13 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaUnsupportedVersion, .hxc_payload.hxc_SchemaUnsupportedVersion.hxc_path = hxc_l_tmp_enum_payload_0_owner_n12, .hxc_payload.hxc_SchemaUnsupportedVersion.hxc_expected = 2 };
    hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n28, hxc_l_tmp_load_result_n29, hxc_l_tmp_instance_call_argument_1_owner_n13);
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n34 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[15] = hxc_l_tmp_call_result_n34.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n34.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n13);
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n34;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n35 = hxc_l_reader;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n36 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n36 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[16] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n38 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n36, hxc_l_fields, (hxc_string){ (const uint8_t *)"packId", 6, true, NULL });
  hxc_l_gc_roots[17] = (const void *)hxc_l_tmp_instance_call_result_n38;
  hxc_string hxc_l_tmp_call_result_n39 = hxc_caxecraft_content_RuntimeContentPack_readContentId(hxc_l_tmp_load_result_n35, hxc_l_tmp_instance_call_result_n38, (hxc_string){ (const uint8_t *)"packId", 6, true, NULL });
  hxc_l_packId = hxc_l_tmp_call_result_n39;
  if (hxc_l_packId.data == NULL)
  {
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n42 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[18] = hxc_l_tmp_call_result_n42.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n42.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n42;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n43 = hxc_l_reader;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n44 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n44 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[19] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n46 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n44, hxc_l_fields, (hxc_string){ (const uint8_t *)"logicalPath", 11, true, NULL });
  hxc_l_gc_roots[20] = (const void *)hxc_l_tmp_instance_call_result_n46;
  hxc_string hxc_l_tmp_call_result_n47 = hxc_caxecraft_content_RuntimeContentPack_readLogicalPath(hxc_l_tmp_load_result_n43, hxc_l_tmp_instance_call_result_n46, (hxc_string){ (const uint8_t *)"logicalPath", 11, true, NULL });
  hxc_l_logicalPath = hxc_l_tmp_call_result_n47;
  if (hxc_l_logicalPath.data == NULL)
  {
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n50 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[21] = hxc_l_tmp_call_result_n50.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n50.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n50;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n51 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n51 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n52 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n52 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[22] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n54 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n52, hxc_l_fields, (hxc_string){ (const uint8_t *)"packVersion", 11, true, NULL });
  hxc_l_gc_roots[23] = (const void *)hxc_l_tmp_instance_call_result_n54;
  struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_instance_call_result_n55 = hxc_caxecraft_content_RuntimeSchemaReader_integer(hxc_l_tmp_load_result_n51, hxc_l_tmp_instance_call_result_n54, (hxc_string){ (const uint8_t *)"packVersion", 11, true, NULL }, 1, 2147483647);
  struct hxc_optional_value_h76f8e80e9d2b hxc_l_packVersion = hxc_l_tmp_instance_call_result_n55;
  if (!hxc_l_packVersion.hxc_has_value)
  {
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n58 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[24] = hxc_l_tmp_call_result_n58.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n58.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n58;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n59 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n59 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[25] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n61 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n59, hxc_l_fields, (hxc_string){ (const uint8_t *)"assetManifestId", 15, true, NULL });
  hxc_l_gc_roots[26] = (const void *)hxc_l_tmp_instance_call_result_n61;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_manifestNode = hxc_l_tmp_instance_call_result_n61;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n62 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n62 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[27] = (const void *)hxc_l_manifestNode;
  hxc_string hxc_l_tmp_instance_call_result_n64 = hxc_caxecraft_content_RuntimeSchemaReader_string(hxc_l_tmp_load_result_n62, hxc_l_manifestNode, (hxc_string){ (const uint8_t *)"assetManifestId", 15, true, NULL }, 128);
  hxc_l_manifestId = hxc_l_tmp_instance_call_result_n64;
  if (hxc_l_manifestId.data == NULL)
  {
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n67 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[28] = hxc_l_tmp_call_result_n67.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n67.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    if (hxc_string_release(&hxc_l_manifestId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n67;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n68 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n68 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[29] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n70 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n68, hxc_l_fields, (hxc_string){ (const uint8_t *)"airBlock", 8, true, NULL });
  hxc_l_gc_roots[30] = (const void *)hxc_l_tmp_instance_call_result_n70;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_airNode = hxc_l_tmp_instance_call_result_n70;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n71 = hxc_l_reader;
  hxc_l_gc_roots[31] = (const void *)hxc_l_airNode;
  hxc_string hxc_l_tmp_call_result_n73 = hxc_caxecraft_content_RuntimeContentPack_readContentId(hxc_l_tmp_load_result_n71, hxc_l_airNode, (hxc_string){ (const uint8_t *)"airBlock", 8, true, NULL });
  hxc_l_air = hxc_l_tmp_call_result_n73;
  if (hxc_l_air.data == NULL)
  {
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n76 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[32] = hxc_l_tmp_call_result_n76.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n76.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    if (hxc_string_release(&hxc_l_air) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_manifestId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n76;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n77 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n77 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[33] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n79 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n77, hxc_l_fields, (hxc_string){ (const uint8_t *)"defaultAquaticProfile", 21, true, NULL });
  hxc_l_gc_roots[34] = (const void *)hxc_l_tmp_instance_call_result_n79;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_defaultAquaticNode = hxc_l_tmp_instance_call_result_n79;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n80 = hxc_l_reader;
  hxc_l_gc_roots[35] = (const void *)hxc_l_defaultAquaticNode;
  hxc_string hxc_l_tmp_call_result_n82 = hxc_caxecraft_content_RuntimeContentPack_readContentId(hxc_l_tmp_load_result_n80, hxc_l_defaultAquaticNode, (hxc_string){ (const uint8_t *)"defaultAquaticProfile", 21, true, NULL });
  hxc_l_defaultAquatic = hxc_l_tmp_call_result_n82;
  if (hxc_l_defaultAquatic.data == NULL)
  {
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n85 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[36] = hxc_l_tmp_call_result_n85.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n85.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    if (hxc_string_release(&hxc_l_defaultAquatic) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_air) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_manifestId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n85;
  }
  hxc_string hxc_l_tmp_load_result_n86 = hxc_l_manifestId;
  if (hxc_l_assets == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_class_field_load_result_n87 = (*hxc_l_assets).hxc_manifestIdValue;
  if (!(hxc_l_tmp_load_result_n86.data == NULL || hxc_l_tmp_class_field_load_result_n87.data == NULL ? hxc_l_tmp_load_result_n86.data == hxc_l_tmp_class_field_load_result_n87.data : hxc_l_tmp_load_result_n86.byte_length == hxc_l_tmp_class_field_load_result_n87.byte_length && (hxc_l_tmp_load_result_n86.byte_length == 0 || memcmp(hxc_l_tmp_load_result_n86.data, hxc_l_tmp_class_field_load_result_n87.data, hxc_l_tmp_load_result_n86.byte_length) == 0)))
  {
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n89 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n89 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n90 = hxc_l_manifestNode;
    hxc_l_gc_roots[37] = (const void *)hxc_l_tmp_load_result_n90;
    hxc_l_tmp_enum_payload_0_owner_n23 = (hxc_string){ (const uint8_t *)"assetManifestId", 15, true, NULL };
    if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n23) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n24 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidInvariant, .hxc_payload.hxc_SchemaInvalidInvariant.hxc_path = hxc_l_tmp_enum_payload_0_owner_n23 };
    hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n89, hxc_l_tmp_load_result_n90, hxc_l_tmp_instance_call_argument_1_owner_n24);
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n95 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[38] = hxc_l_tmp_call_result_n95.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n95.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n24);
    if (hxc_string_release(&hxc_l_defaultAquatic) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_air) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_manifestId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n95;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n96 = hxc_l_reader;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n97 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n97 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[39] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n99 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n97, hxc_l_fields, (hxc_string){ (const uint8_t *)"features", 8, true, NULL });
  hxc_l_gc_roots[40] = (const void *)hxc_l_tmp_instance_call_result_n99;
  struct hxc_array_ref *hxc_l_tmp_call_result_n100 = hxc_caxecraft_content_RuntimeContentPack_readIdArray(hxc_l_tmp_load_result_n96, hxc_l_tmp_instance_call_result_n99, (hxc_string){ (const uint8_t *)"features", 8, true, NULL }, 1, 32);
  hxc_l_gc_roots[41] = (const void *)hxc_l_tmp_call_result_n100;
  struct hxc_array_ref *hxc_l_features = hxc_l_tmp_call_result_n100;
  hxc_l_gc_roots[42] = (const void *)hxc_l_features;
  hxc_l_gc_roots[43] = (const void *)NULL;
  if (hxc_l_features == NULL)
  {
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n103 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[44] = hxc_l_tmp_call_result_n103.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n103.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    if (hxc_string_release(&hxc_l_defaultAquatic) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_air) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_manifestId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n103;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n104 = hxc_l_reader;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n105 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n105 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[45] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n107 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n105, hxc_l_fields, (hxc_string){ (const uint8_t *)"blocks", 6, true, NULL });
  hxc_l_gc_roots[46] = (const void *)hxc_l_tmp_instance_call_result_n107;
  struct hxc_array_ref *hxc_l_tmp_call_result_n108 = hxc_caxecraft_content_RuntimeContentPack_readBlocks(hxc_l_tmp_load_result_n104, hxc_l_tmp_instance_call_result_n107);
  hxc_l_gc_roots[47] = (const void *)hxc_l_tmp_call_result_n108;
  struct hxc_array_ref *hxc_l_blocks = hxc_l_tmp_call_result_n108;
  hxc_l_gc_roots[48] = (const void *)hxc_l_blocks;
  hxc_l_gc_roots[49] = (const void *)NULL;
  if (hxc_l_blocks == NULL)
  {
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n111 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[50] = hxc_l_tmp_call_result_n111.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n111.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    if (hxc_string_release(&hxc_l_defaultAquatic) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_air) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_manifestId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n111;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n112 = hxc_l_reader;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n113 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n113 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[51] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n115 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n113, hxc_l_fields, (hxc_string){ (const uint8_t *)"fluids", 6, true, NULL });
  hxc_l_gc_roots[52] = (const void *)hxc_l_tmp_instance_call_result_n115;
  struct hxc_array_ref *hxc_l_tmp_call_result_n116 = hxc_caxecraft_content_RuntimeContentPack_readFluids(hxc_l_tmp_load_result_n112, hxc_l_tmp_instance_call_result_n115, hxc_l_assets);
  hxc_l_gc_roots[53] = (const void *)hxc_l_tmp_call_result_n116;
  struct hxc_array_ref *hxc_l_fluids = hxc_l_tmp_call_result_n116;
  hxc_l_gc_roots[54] = (const void *)hxc_l_fluids;
  hxc_l_gc_roots[55] = (const void *)NULL;
  if (hxc_l_fluids == NULL)
  {
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n119 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[56] = hxc_l_tmp_call_result_n119.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n119.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    if (hxc_string_release(&hxc_l_defaultAquatic) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_air) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_manifestId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n119;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n120 = hxc_l_reader;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n121 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n121 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[57] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n123 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n121, hxc_l_fields, (hxc_string){ (const uint8_t *)"aquaticProfiles", 15, true, NULL });
  hxc_l_gc_roots[58] = (const void *)hxc_l_tmp_instance_call_result_n123;
  struct hxc_array_ref *hxc_l_tmp_call_result_n124 = hxc_caxecraft_content_RuntimeContentPack_readAquaticProfiles(hxc_l_tmp_load_result_n120, hxc_l_tmp_instance_call_result_n123);
  hxc_l_gc_roots[59] = (const void *)hxc_l_tmp_call_result_n124;
  struct hxc_array_ref *hxc_l_aquaticProfiles = hxc_l_tmp_call_result_n124;
  hxc_l_gc_roots[60] = (const void *)hxc_l_aquaticProfiles;
  hxc_l_gc_roots[61] = (const void *)NULL;
  if (hxc_l_aquaticProfiles == NULL)
  {
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n127 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[62] = hxc_l_tmp_call_result_n127.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n127.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    if (hxc_string_release(&hxc_l_defaultAquatic) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_air) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_manifestId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n127;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n128 = hxc_l_reader;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n129 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n129 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[63] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n131 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n129, hxc_l_fields, (hxc_string){ (const uint8_t *)"items", 5, true, NULL });
  hxc_l_gc_roots[64] = (const void *)hxc_l_tmp_instance_call_result_n131;
  struct hxc_array_ref *hxc_l_tmp_call_result_n132 = hxc_caxecraft_content_RuntimeContentPack_readItems(hxc_l_tmp_load_result_n128, hxc_l_tmp_instance_call_result_n131, hxc_l_assets);
  hxc_l_gc_roots[65] = (const void *)hxc_l_tmp_call_result_n132;
  struct hxc_array_ref *hxc_l_items = hxc_l_tmp_call_result_n132;
  hxc_l_gc_roots[66] = (const void *)hxc_l_items;
  hxc_l_gc_roots[67] = (const void *)NULL;
  if (hxc_l_items == NULL)
  {
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n135 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[68] = hxc_l_tmp_call_result_n135.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n135.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    if (hxc_string_release(&hxc_l_defaultAquatic) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_air) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_manifestId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n135;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n136 = hxc_l_reader;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n137 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n137 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[69] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n139 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n137, hxc_l_fields, (hxc_string){ (const uint8_t *)"npcs", 4, true, NULL });
  hxc_l_gc_roots[70] = (const void *)hxc_l_tmp_instance_call_result_n139;
  struct hxc_array_ref *hxc_l_tmp_call_result_n140 = hxc_caxecraft_content_RuntimeContentPack_readNpcs(hxc_l_tmp_load_result_n136, hxc_l_tmp_instance_call_result_n139, hxc_l_assets);
  hxc_l_gc_roots[71] = (const void *)hxc_l_tmp_call_result_n140;
  struct hxc_array_ref *hxc_l_npcs = hxc_l_tmp_call_result_n140;
  hxc_l_gc_roots[72] = (const void *)hxc_l_npcs;
  hxc_l_gc_roots[73] = (const void *)NULL;
  if (hxc_l_npcs == NULL)
  {
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n143 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[74] = hxc_l_tmp_call_result_n143.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n143.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    if (hxc_string_release(&hxc_l_defaultAquatic) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_air) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_manifestId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n143;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n144 = hxc_l_reader;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n145 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n145 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[75] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n147 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n145, hxc_l_fields, (hxc_string){ (const uint8_t *)"enemies", 7, true, NULL });
  hxc_l_gc_roots[76] = (const void *)hxc_l_tmp_instance_call_result_n147;
  struct hxc_array_ref *hxc_l_tmp_call_result_n148 = hxc_caxecraft_content_RuntimeContentPack_readEnemies(hxc_l_tmp_load_result_n144, hxc_l_tmp_instance_call_result_n147, hxc_l_assets);
  hxc_l_gc_roots[77] = (const void *)hxc_l_tmp_call_result_n148;
  struct hxc_array_ref *hxc_l_enemies = hxc_l_tmp_call_result_n148;
  hxc_l_gc_roots[78] = (const void *)hxc_l_enemies;
  hxc_l_gc_roots[79] = (const void *)NULL;
  if (hxc_l_enemies == NULL)
  {
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n151 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[80] = hxc_l_tmp_call_result_n151.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n151.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    if (hxc_string_release(&hxc_l_defaultAquatic) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_air) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_manifestId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n151;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n152 = hxc_l_reader;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n153 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n153 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[81] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n155 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n153, hxc_l_fields, (hxc_string){ (const uint8_t *)"drops", 5, true, NULL });
  hxc_l_gc_roots[82] = (const void *)hxc_l_tmp_instance_call_result_n155;
  struct hxc_array_ref *hxc_l_tmp_call_result_n156 = hxc_caxecraft_content_RuntimeContentPack_readDrops(hxc_l_tmp_load_result_n152, hxc_l_tmp_instance_call_result_n155, hxc_l_assets);
  hxc_l_gc_roots[83] = (const void *)hxc_l_tmp_call_result_n156;
  struct hxc_array_ref *hxc_l_drops = hxc_l_tmp_call_result_n156;
  hxc_l_gc_roots[84] = (const void *)hxc_l_drops;
  hxc_l_gc_roots[85] = (const void *)NULL;
  if (hxc_l_drops == NULL)
  {
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n159 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[86] = hxc_l_tmp_call_result_n159.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n159.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    if (hxc_string_release(&hxc_l_defaultAquatic) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_air) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_manifestId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n159;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n160 = hxc_l_reader;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n161 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n161 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[87] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n163 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n161, hxc_l_fields, (hxc_string){ (const uint8_t *)"effects", 7, true, NULL });
  hxc_l_gc_roots[88] = (const void *)hxc_l_tmp_instance_call_result_n163;
  struct hxc_array_ref *hxc_l_tmp_call_result_n164 = hxc_caxecraft_content_RuntimeContentPack_readEffects(hxc_l_tmp_load_result_n160, hxc_l_tmp_instance_call_result_n163);
  hxc_l_gc_roots[89] = (const void *)hxc_l_tmp_call_result_n164;
  struct hxc_array_ref *hxc_l_effects = hxc_l_tmp_call_result_n164;
  hxc_l_gc_roots[90] = (const void *)hxc_l_effects;
  hxc_l_gc_roots[91] = (const void *)NULL;
  if (hxc_l_effects == NULL)
  {
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n167 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[92] = hxc_l_tmp_call_result_n167.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n167.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    if (hxc_string_release(&hxc_l_defaultAquatic) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_air) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_manifestId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n167;
  }
  hxc_l_reserved_h8da772c5a155 = (hxc_string){ (const uint8_t *)"prefabs", 7, true, NULL };
  if (hxc_string_retain(hxc_l_reserved_h8da772c5a155) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n168 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n168 == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_load_result_n169 = hxc_l_fields;
  hxc_l_gc_roots[93] = (const void *)hxc_l_tmp_load_result_n169;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n171 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n168, hxc_l_tmp_load_result_n169, hxc_l_reserved_h8da772c5a155);
  hxc_l_gc_roots[94] = (const void *)hxc_l_tmp_instance_call_result_n171;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_reservedNode_h4c55b3db7b15 = hxc_l_tmp_instance_call_result_n171;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n172 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n172 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n173 = hxc_l_reservedNode_h4c55b3db7b15;
  hxc_l_gc_roots[95] = (const void *)hxc_l_tmp_load_result_n173;
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n175 = hxc_caxecraft_content_RuntimeSchemaReader_array(hxc_l_tmp_load_result_n172, hxc_l_tmp_load_result_n173, hxc_l_reserved_h8da772c5a155, 0, 128);
  hxc_l_gc_roots[96] = (const void *)hxc_l_tmp_instance_call_result_n175;
  struct hxc_array_ref *hxc_l_values_hc464ca004db1 = hxc_l_tmp_instance_call_result_n175;
  hxc_l_gc_roots[97] = (const void *)hxc_l_values_hc464ca004db1;
  hxc_l_gc_roots[98] = (const void *)NULL;
  if (hxc_l_values_hc464ca004db1 == NULL)
  {
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n178 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[99] = hxc_l_tmp_call_result_n178.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n178.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    if (hxc_string_release(&hxc_l_reserved_h8da772c5a155) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_defaultAquatic) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_air) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_manifestId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n178;
  }
  hxc_l_gc_roots[100] = (const void *)hxc_l_values_hc464ca004db1;
  int32_t hxc_l_tmp_array_length_result_n180;
  if (hxc_array_ref_length(hxc_l_values_hc464ca004db1, &hxc_l_tmp_array_length_result_n180) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_tmp_array_length_result_n180 != 0)
  {
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n181 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n181 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n182 = hxc_l_reservedNode_h4c55b3db7b15;
    hxc_l_gc_roots[101] = (const void *)hxc_l_tmp_load_result_n182;
    hxc_l_tmp_enum_payload_0_owner_n37 = hxc_l_reserved_h8da772c5a155;
    if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n37) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n38 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaUnsupportedReservedKind, .hxc_payload.hxc_SchemaUnsupportedReservedKind.hxc_path = hxc_l_tmp_enum_payload_0_owner_n37 };
    hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n181, hxc_l_tmp_load_result_n182, hxc_l_tmp_instance_call_argument_1_owner_n38);
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n188 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[102] = hxc_l_tmp_call_result_n188.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n188.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n38);
    if (hxc_string_release(&hxc_l_reserved_h8da772c5a155) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_defaultAquatic) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_air) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_manifestId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n188;
  }
  hxc_l_reserved_h16797aa67ceb = (hxc_string){ (const uint8_t *)"statefulObjects", 15, true, NULL };
  if (hxc_string_retain(hxc_l_reserved_h16797aa67ceb) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n189 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n189 == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_load_result_n190 = hxc_l_fields;
  hxc_l_gc_roots[103] = (const void *)hxc_l_tmp_load_result_n190;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n192 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n189, hxc_l_tmp_load_result_n190, hxc_l_reserved_h16797aa67ceb);
  hxc_l_gc_roots[104] = (const void *)hxc_l_tmp_instance_call_result_n192;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_reservedNode_h11f4c0d546b4 = hxc_l_tmp_instance_call_result_n192;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n193 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n193 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n194 = hxc_l_reservedNode_h11f4c0d546b4;
  hxc_l_gc_roots[105] = (const void *)hxc_l_tmp_load_result_n194;
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n196 = hxc_caxecraft_content_RuntimeSchemaReader_array(hxc_l_tmp_load_result_n193, hxc_l_tmp_load_result_n194, hxc_l_reserved_h16797aa67ceb, 0, 128);
  hxc_l_gc_roots[106] = (const void *)hxc_l_tmp_instance_call_result_n196;
  struct hxc_array_ref *hxc_l_values_h516e06d5cf22 = hxc_l_tmp_instance_call_result_n196;
  hxc_l_gc_roots[107] = (const void *)hxc_l_values_h516e06d5cf22;
  hxc_l_gc_roots[108] = (const void *)NULL;
  if (hxc_l_values_h516e06d5cf22 == NULL)
  {
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n199 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[109] = hxc_l_tmp_call_result_n199.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n199.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    if (hxc_string_release(&hxc_l_reserved_h16797aa67ceb) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_reserved_h8da772c5a155) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_defaultAquatic) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_air) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_manifestId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n199;
  }
  hxc_l_gc_roots[110] = (const void *)hxc_l_values_h516e06d5cf22;
  int32_t hxc_l_tmp_array_length_result_n201;
  if (hxc_array_ref_length(hxc_l_values_h516e06d5cf22, &hxc_l_tmp_array_length_result_n201) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_tmp_array_length_result_n201 != 0)
  {
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n202 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n202 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n203 = hxc_l_reservedNode_h11f4c0d546b4;
    hxc_l_gc_roots[111] = (const void *)hxc_l_tmp_load_result_n203;
    hxc_l_tmp_enum_payload_0_owner_n42 = hxc_l_reserved_h16797aa67ceb;
    if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n42) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n43 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaUnsupportedReservedKind, .hxc_payload.hxc_SchemaUnsupportedReservedKind.hxc_path = hxc_l_tmp_enum_payload_0_owner_n42 };
    hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n202, hxc_l_tmp_load_result_n203, hxc_l_tmp_instance_call_argument_1_owner_n43);
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n209 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[112] = hxc_l_tmp_call_result_n209.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n209.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n43);
    if (hxc_string_release(&hxc_l_reserved_h16797aa67ceb) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_reserved_h8da772c5a155) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_defaultAquatic) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_air) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_manifestId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n209;
  }
  hxc_l_reserved_h8fce842889d2 = (hxc_string){ (const uint8_t *)"states", 6, true, NULL };
  if (hxc_string_retain(hxc_l_reserved_h8fce842889d2) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n210 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n210 == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_load_result_n211 = hxc_l_fields;
  hxc_l_gc_roots[113] = (const void *)hxc_l_tmp_load_result_n211;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n213 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n210, hxc_l_tmp_load_result_n211, hxc_l_reserved_h8fce842889d2);
  hxc_l_gc_roots[114] = (const void *)hxc_l_tmp_instance_call_result_n213;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_reservedNode_h83e600480c61 = hxc_l_tmp_instance_call_result_n213;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n214 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n214 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n215 = hxc_l_reservedNode_h83e600480c61;
  hxc_l_gc_roots[115] = (const void *)hxc_l_tmp_load_result_n215;
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n217 = hxc_caxecraft_content_RuntimeSchemaReader_array(hxc_l_tmp_load_result_n214, hxc_l_tmp_load_result_n215, hxc_l_reserved_h8fce842889d2, 0, 128);
  hxc_l_gc_roots[116] = (const void *)hxc_l_tmp_instance_call_result_n217;
  struct hxc_array_ref *hxc_l_values_h5c2fa55e8867 = hxc_l_tmp_instance_call_result_n217;
  hxc_l_gc_roots[117] = (const void *)hxc_l_values_h5c2fa55e8867;
  hxc_l_gc_roots[118] = (const void *)NULL;
  if (hxc_l_values_h5c2fa55e8867 == NULL)
  {
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n220 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[119] = hxc_l_tmp_call_result_n220.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n220.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    if (hxc_string_release(&hxc_l_reserved_h8fce842889d2) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_reserved_h16797aa67ceb) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_reserved_h8da772c5a155) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_defaultAquatic) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_air) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_manifestId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n220;
  }
  hxc_l_gc_roots[120] = (const void *)hxc_l_values_h5c2fa55e8867;
  int32_t hxc_l_tmp_array_length_result_n222;
  if (hxc_array_ref_length(hxc_l_values_h5c2fa55e8867, &hxc_l_tmp_array_length_result_n222) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_tmp_array_length_result_n222 != 0)
  {
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n223 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n223 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n224 = hxc_l_reservedNode_h83e600480c61;
    hxc_l_gc_roots[121] = (const void *)hxc_l_tmp_load_result_n224;
    hxc_l_tmp_enum_payload_0_owner_n47 = hxc_l_reserved_h8fce842889d2;
    if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n47) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n48 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaUnsupportedReservedKind, .hxc_payload.hxc_SchemaUnsupportedReservedKind.hxc_path = hxc_l_tmp_enum_payload_0_owner_n47 };
    hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n223, hxc_l_tmp_load_result_n224, hxc_l_tmp_instance_call_argument_1_owner_n48);
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n230 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[122] = hxc_l_tmp_call_result_n230.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n230.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n48);
    if (hxc_string_release(&hxc_l_reserved_h8fce842889d2) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_reserved_h16797aa67ceb) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_reserved_h8da772c5a155) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_defaultAquatic) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_air) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_manifestId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n230;
  }
  hxc_l_reserved_h36c4a708a71d = (hxc_string){ (const uint8_t *)"signals", 7, true, NULL };
  if (hxc_string_retain(hxc_l_reserved_h36c4a708a71d) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n231 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n231 == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_load_result_n232 = hxc_l_fields;
  hxc_l_gc_roots[123] = (const void *)hxc_l_tmp_load_result_n232;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n234 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n231, hxc_l_tmp_load_result_n232, hxc_l_reserved_h36c4a708a71d);
  hxc_l_gc_roots[124] = (const void *)hxc_l_tmp_instance_call_result_n234;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_reservedNode_h92164bb6192b = hxc_l_tmp_instance_call_result_n234;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n235 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n235 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n236 = hxc_l_reservedNode_h92164bb6192b;
  hxc_l_gc_roots[125] = (const void *)hxc_l_tmp_load_result_n236;
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n238 = hxc_caxecraft_content_RuntimeSchemaReader_array(hxc_l_tmp_load_result_n235, hxc_l_tmp_load_result_n236, hxc_l_reserved_h36c4a708a71d, 0, 128);
  hxc_l_gc_roots[126] = (const void *)hxc_l_tmp_instance_call_result_n238;
  struct hxc_array_ref *hxc_l_values_hf38a143b168f = hxc_l_tmp_instance_call_result_n238;
  hxc_l_gc_roots[127] = (const void *)hxc_l_values_hf38a143b168f;
  hxc_l_gc_roots[128] = (const void *)NULL;
  if (hxc_l_values_hf38a143b168f == NULL)
  {
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n241 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[129] = hxc_l_tmp_call_result_n241.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n241.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    if (hxc_string_release(&hxc_l_reserved_h36c4a708a71d) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_reserved_h8fce842889d2) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_reserved_h16797aa67ceb) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_reserved_h8da772c5a155) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_defaultAquatic) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_air) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_manifestId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n241;
  }
  hxc_l_gc_roots[130] = (const void *)hxc_l_values_hf38a143b168f;
  int32_t hxc_l_tmp_array_length_result_n243;
  if (hxc_array_ref_length(hxc_l_values_hf38a143b168f, &hxc_l_tmp_array_length_result_n243) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_tmp_array_length_result_n243 != 0)
  {
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n244 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n244 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n245 = hxc_l_reservedNode_h92164bb6192b;
    hxc_l_gc_roots[131] = (const void *)hxc_l_tmp_load_result_n245;
    hxc_l_tmp_enum_payload_0_owner_n52 = hxc_l_reserved_h36c4a708a71d;
    if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n52) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n53 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaUnsupportedReservedKind, .hxc_payload.hxc_SchemaUnsupportedReservedKind.hxc_path = hxc_l_tmp_enum_payload_0_owner_n52 };
    hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n244, hxc_l_tmp_load_result_n245, hxc_l_tmp_instance_call_argument_1_owner_n53);
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n251 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[132] = hxc_l_tmp_call_result_n251.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n251.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n53);
    if (hxc_string_release(&hxc_l_reserved_h36c4a708a71d) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_reserved_h8fce842889d2) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_reserved_h16797aa67ceb) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_reserved_h8da772c5a155) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_defaultAquatic) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_air) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_manifestId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n251;
  }
  hxc_l_gc_roots[133] = (const void *)hxc_l_features;
  int32_t hxc_l_tmp_array_length_result_n253;
  if (hxc_array_ref_length(hxc_l_features, &hxc_l_tmp_array_length_result_n253) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_tmp_short_circuit_result_n54 = hxc_l_tmp_array_length_result_n253 != 1;
  if (!(hxc_l_tmp_array_length_result_n253 != 1))
  {
    hxc_l_gc_roots[134] = (const void *)hxc_l_features;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeLocatedId *hxc_l_tmp_array_get_result_n255;
    if (hxc_array_ref_get_copy(hxc_l_features, (size_t)0, &hxc_l_tmp_array_get_result_n255) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[135] = (const void *)hxc_l_tmp_array_get_result_n255;
    if (hxc_l_tmp_array_get_result_n255 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n256 = (*hxc_l_tmp_array_get_result_n255).hxc_id;
    hxc_l_tmp_short_circuit_result_n54 = !(hxc_l_tmp_class_field_load_result_n256.data != NULL && (hxc_l_tmp_class_field_load_result_n256.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:core", 14, true, NULL }.byte_length && (hxc_l_tmp_class_field_load_result_n256.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n256.data, (hxc_string){ (const uint8_t *)"caxecraft:core", 14, true, NULL }.data, hxc_l_tmp_class_field_load_result_n256.byte_length) == 0)));
  }
  if (hxc_l_tmp_short_circuit_result_n54)
  {
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n259 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n259 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n260 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n260 == NULL)
    {
      abort();
    }
    hxc_l_gc_roots[136] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n262 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n260, hxc_l_fields, (hxc_string){ (const uint8_t *)"features", 8, true, NULL });
    hxc_l_gc_roots[137] = (const void *)hxc_l_tmp_instance_call_result_n262;
    hxc_l_tmp_enum_payload_0_owner_n55 = (hxc_string){ (const uint8_t *)"features", 8, true, NULL };
    if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n55) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n56 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidInvariant, .hxc_payload.hxc_SchemaInvalidInvariant.hxc_path = hxc_l_tmp_enum_payload_0_owner_n55 };
    hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n259, hxc_l_tmp_instance_call_result_n262, hxc_l_tmp_instance_call_argument_1_owner_n56);
    struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n267 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
    hxc_l_gc_roots[138] = hxc_l_tmp_call_result_n267.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n267.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n56);
    if (hxc_string_release(&hxc_l_reserved_h36c4a708a71d) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_reserved_h8fce842889d2) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_reserved_h16797aa67ceb) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_reserved_h8da772c5a155) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_defaultAquatic) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_air) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_manifestId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n267;
  }
  hxc_string hxc_l_tmp_load_result_n268 = hxc_l_air;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n269 = hxc_l_airNode;
  hxc_l_gc_roots[139] = (const void *)hxc_l_tmp_load_result_n269;
  if (hxc_l_tmp_load_result_n269 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n270 = (*hxc_l_tmp_load_result_n269).hxc_line;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n271 = hxc_l_airNode;
  hxc_l_gc_roots[140] = (const void *)hxc_l_tmp_load_result_n271;
  if (hxc_l_tmp_load_result_n271 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n272 = (*hxc_l_tmp_load_result_n271).hxc_column;
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_tmp_managed_class_result_n273 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_RuntimeContentPack_RuntimeReference_descriptor, (void **)&hxc_l_tmp_managed_class_result_n273) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[141] = (const void *)hxc_l_tmp_managed_class_result_n273;
  hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeReference(hxc_l_tmp_managed_class_result_n273, hxc_l_tmp_load_result_n268, hxc_l_tmp_class_field_load_result_n270, hxc_l_tmp_class_field_load_result_n272);
  hxc_l_gc_roots[142] = (const void *)hxc_l_tmp_managed_class_result_n273;
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_airReference = hxc_l_tmp_managed_class_result_n273;
  hxc_string hxc_l_tmp_load_result_n274 = hxc_l_defaultAquatic;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n275 = hxc_l_defaultAquaticNode;
  hxc_l_gc_roots[143] = (const void *)hxc_l_tmp_load_result_n275;
  if (hxc_l_tmp_load_result_n275 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n276 = (*hxc_l_tmp_load_result_n275).hxc_line;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n277 = hxc_l_defaultAquaticNode;
  hxc_l_gc_roots[144] = (const void *)hxc_l_tmp_load_result_n277;
  if (hxc_l_tmp_load_result_n277 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n278 = (*hxc_l_tmp_load_result_n277).hxc_column;
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_tmp_managed_class_result_n279 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_RuntimeContentPack_RuntimeReference_descriptor, (void **)&hxc_l_tmp_managed_class_result_n279) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[145] = (const void *)hxc_l_tmp_managed_class_result_n279;
  hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeReference(hxc_l_tmp_managed_class_result_n279, hxc_l_tmp_load_result_n274, hxc_l_tmp_class_field_load_result_n276, hxc_l_tmp_class_field_load_result_n278);
  hxc_l_gc_roots[146] = (const void *)hxc_l_tmp_managed_class_result_n279;
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_defaultAquaticReference = hxc_l_tmp_managed_class_result_n279;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n280 = hxc_l_reader;
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_tmp_load_result_n281 = hxc_l_airReference;
  hxc_l_gc_roots[147] = (const void *)hxc_l_tmp_load_result_n281;
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_tmp_load_result_n282 = hxc_l_defaultAquaticReference;
  hxc_l_gc_roots[148] = (const void *)hxc_l_tmp_load_result_n282;
  struct hxc_array_ref *hxc_l_tmp_load_result_n283 = hxc_l_features;
  hxc_l_gc_roots[149] = (const void *)hxc_l_tmp_load_result_n283;
  struct hxc_array_ref *hxc_l_tmp_load_result_n284 = hxc_l_blocks;
  hxc_l_gc_roots[150] = (const void *)hxc_l_tmp_load_result_n284;
  struct hxc_array_ref *hxc_l_tmp_load_result_n285 = hxc_l_fluids;
  hxc_l_gc_roots[151] = (const void *)hxc_l_tmp_load_result_n285;
  struct hxc_array_ref *hxc_l_tmp_load_result_n286 = hxc_l_aquaticProfiles;
  hxc_l_gc_roots[152] = (const void *)hxc_l_tmp_load_result_n286;
  struct hxc_array_ref *hxc_l_tmp_load_result_n287 = hxc_l_items;
  hxc_l_gc_roots[153] = (const void *)hxc_l_tmp_load_result_n287;
  struct hxc_array_ref *hxc_l_tmp_load_result_n288 = hxc_l_npcs;
  hxc_l_gc_roots[154] = (const void *)hxc_l_tmp_load_result_n288;
  struct hxc_array_ref *hxc_l_tmp_load_result_n289 = hxc_l_enemies;
  hxc_l_gc_roots[155] = (const void *)hxc_l_tmp_load_result_n289;
  struct hxc_array_ref *hxc_l_tmp_load_result_n290 = hxc_l_drops;
  hxc_l_gc_roots[156] = (const void *)hxc_l_tmp_load_result_n290;
  hxc_l_gc_roots[157] = (const void *)hxc_l_effects;
  bool hxc_l_tmp_call_result_n292 = hxc_caxecraft_content_RuntimeContentPack_validatePack(hxc_l_tmp_load_result_n280, hxc_l_tmp_load_result_n281, hxc_l_tmp_load_result_n282, hxc_l_tmp_load_result_n283, hxc_l_tmp_load_result_n284, hxc_l_tmp_load_result_n285, hxc_l_tmp_load_result_n286, hxc_l_tmp_load_result_n287, hxc_l_tmp_load_result_n288, hxc_l_tmp_load_result_n289, hxc_l_tmp_load_result_n290, hxc_l_effects);
  if (!!hxc_l_tmp_call_result_n292)
  {
    hxc_string hxc_l_tmp_load_result_n295 = hxc_l_packId;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n296 = hxc_l_packVersion;
    if (!hxc_l_tmp_load_result_n296.hxc_has_value)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n298 = hxc_l_logicalPath;
    hxc_string hxc_l_tmp_load_result_n299 = hxc_l_air;
    hxc_string hxc_l_tmp_load_result_n300 = hxc_l_defaultAquatic;
    struct hxc_array_ref *hxc_l_tmp_load_result_n301 = hxc_l_features;
    hxc_l_gc_roots[159] = (const void *)hxc_l_tmp_load_result_n301;
    struct hxc_array_ref *hxc_l_tmp_load_result_n302 = hxc_l_blocks;
    hxc_l_gc_roots[160] = (const void *)hxc_l_tmp_load_result_n302;
    struct hxc_array_ref *hxc_l_tmp_load_result_n303 = hxc_l_fluids;
    hxc_l_gc_roots[161] = (const void *)hxc_l_tmp_load_result_n303;
    struct hxc_array_ref *hxc_l_tmp_load_result_n304 = hxc_l_aquaticProfiles;
    hxc_l_gc_roots[162] = (const void *)hxc_l_tmp_load_result_n304;
    struct hxc_array_ref *hxc_l_tmp_load_result_n305 = hxc_l_items;
    hxc_l_gc_roots[163] = (const void *)hxc_l_tmp_load_result_n305;
    struct hxc_array_ref *hxc_l_tmp_load_result_n306 = hxc_l_npcs;
    hxc_l_gc_roots[164] = (const void *)hxc_l_tmp_load_result_n306;
    struct hxc_array_ref *hxc_l_tmp_load_result_n307 = hxc_l_enemies;
    hxc_l_gc_roots[165] = (const void *)hxc_l_tmp_load_result_n307;
    struct hxc_array_ref *hxc_l_tmp_load_result_n308 = hxc_l_drops;
    hxc_l_gc_roots[166] = (const void *)hxc_l_tmp_load_result_n308;
    struct hxc_array_ref *hxc_l_tmp_load_result_n309 = hxc_l_effects;
    hxc_l_gc_roots[167] = (const void *)hxc_l_tmp_load_result_n309;
    struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_tmp_managed_class_result_n310 = NULL;
    if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_RuntimeContentRegistry_descriptor, (void **)&hxc_l_tmp_managed_class_result_n310) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[168] = (const void *)hxc_l_tmp_managed_class_result_n310;
    hxc_compiler_constructor_caxecraft_content_RuntimeContentRegistry(hxc_l_tmp_managed_class_result_n310, hxc_l_tmp_load_result_n295, hxc_l_tmp_load_result_n296.hxc_value, hxc_l_tmp_load_result_n298, hxc_l_tmp_load_result_n299, hxc_l_tmp_load_result_n300, hxc_l_tmp_load_result_n301, hxc_l_tmp_load_result_n302, hxc_l_tmp_load_result_n303, hxc_l_tmp_load_result_n304, hxc_l_tmp_load_result_n305, hxc_l_tmp_load_result_n306, hxc_l_tmp_load_result_n307, hxc_l_tmp_load_result_n308, hxc_l_tmp_load_result_n309);
    hxc_l_gc_roots[169] = (const void *)hxc_l_tmp_managed_class_result_n310;
    hxc_l_gc_roots[170] = (struct hxc_caxecraft_content_RuntimeContentPackResult){ .hxc_tag = hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady, .hxc_payload.hxc_RuntimeContentPackReady.hxc_registry = hxc_l_tmp_managed_class_result_n310 }.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)(struct hxc_caxecraft_content_RuntimeContentPackResult){ .hxc_tag = hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady, .hxc_payload.hxc_RuntimeContentPackReady.hxc_registry = hxc_l_tmp_managed_class_result_n310 }.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    if (hxc_string_release(&hxc_l_reserved_h36c4a708a71d) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_reserved_h8fce842889d2) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_reserved_h16797aa67ceb) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_reserved_h8da772c5a155) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_defaultAquatic) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_air) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_manifestId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_RuntimeContentPackResult){ .hxc_tag = hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady, .hxc_payload.hxc_RuntimeContentPackReady.hxc_registry = hxc_l_tmp_managed_class_result_n310 };
  }
  struct hxc_caxecraft_content_RuntimeContentPackResult hxc_l_tmp_call_result_n294 = hxc_caxecraft_content_RuntimeContentPack_rejected(hxc_l_reader);
  hxc_l_gc_roots[158] = hxc_l_tmp_call_result_n294.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)hxc_l_tmp_call_result_n294.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
  if (hxc_string_release(&hxc_l_reserved_h36c4a708a71d) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_reserved_h8fce842889d2) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_reserved_h16797aa67ceb) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_reserved_h8da772c5a155) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_defaultAquatic) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_air) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_manifestId) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_logicalPath) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_packId) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n6) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n3.hxc_failure);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_call_result_n294;
}

struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_caxecraft_content_RuntimeContentPack_findItemDefinition(struct hxc_array_ref *hxc_l_values, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[5] = { (const void *)hxc_l_values, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 5, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n0 = hxc_l_g;
    int32_t hxc_l_tmp_array_length_result_n1;
    if (hxc_array_ref_length(hxc_l_values, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n0 < hxc_l_tmp_array_length_result_n1))
    {
      break;
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_tmp_array_get_result_n3;
    if (hxc_array_ref_get_copy(hxc_l_values, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_array_get_result_n3;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_value = hxc_l_tmp_array_get_result_n3;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_tmp_load_result_n5 = hxc_l_value;
    hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_load_result_n5;
    if (hxc_l_tmp_load_result_n5 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n6 = (*hxc_l_tmp_load_result_n5).hxc_base.hxc_id;
    if (hxc_l_tmp_class_field_load_result_n6.data == NULL || hxc_l_id.data == NULL ? hxc_l_tmp_class_field_load_result_n6.data == hxc_l_id.data : hxc_l_tmp_class_field_load_result_n6.byte_length == hxc_l_id.byte_length && (hxc_l_tmp_class_field_load_result_n6.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n6.data, hxc_l_id.data, hxc_l_tmp_class_field_load_result_n6.byte_length) == 0))
    {
      hxc_l_gc_roots[4] = (const void *)hxc_l_value;
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_value;
    }
  }
  hxc_l_gc_roots[3] = (const void *)NULL;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return NULL;
}

enum hxc_caxecraft_content_RuntimeItemUseProfile hxc_caxecraft_content_RuntimeContentPack_itemUse(hxc_string hxc_l_value)
{
  if (hxc_l_value.data != NULL && (hxc_l_value.byte_length == (hxc_string){ (const uint8_t *)"consume-one-heart", 17, true, NULL }.byte_length && (hxc_l_value.byte_length == 0 || memcmp(hxc_l_value.data, (hxc_string){ (const uint8_t *)"consume-one-heart", 17, true, NULL }.data, hxc_l_value.byte_length) == 0)))
  {
    return hxc_caxecraft_content_RuntimeItemUseProfile_ConsumeOneHeart;
  }
  if (hxc_l_value.data != NULL && (hxc_l_value.byte_length == (hxc_string){ (const uint8_t *)"equip-aquatic", 13, true, NULL }.byte_length && (hxc_l_value.byte_length == 0 || memcmp(hxc_l_value.data, (hxc_string){ (const uint8_t *)"equip-aquatic", 13, true, NULL }.data, hxc_l_value.byte_length) == 0)))
  {
    return hxc_caxecraft_content_RuntimeItemUseProfile_EquipAquatic;
  }
  if (hxc_l_value.data != NULL && (hxc_l_value.byte_length == (hxc_string){ (const uint8_t *)"haxeforge-tool", 14, true, NULL }.byte_length && (hxc_l_value.byte_length == 0 || memcmp(hxc_l_value.data, (hxc_string){ (const uint8_t *)"haxeforge-tool", 14, true, NULL }.data, hxc_l_value.byte_length) == 0)))
  {
    return hxc_caxecraft_content_RuntimeItemUseProfile_HaxeforgeTool;
  }
  if (hxc_l_value.data != NULL && (hxc_l_value.byte_length == (hxc_string){ (const uint8_t *)"light-source", 12, true, NULL }.byte_length && (hxc_l_value.byte_length == 0 || memcmp(hxc_l_value.data, (hxc_string){ (const uint8_t *)"light-source", 12, true, NULL }.data, hxc_l_value.byte_length) == 0)))
  {
    return hxc_caxecraft_content_RuntimeItemUseProfile_LightSource;
  }
  if (hxc_l_value.data != NULL && (hxc_l_value.byte_length == (hxc_string){ (const uint8_t *)"melee-sword", 11, true, NULL }.byte_length && (hxc_l_value.byte_length == 0 || memcmp(hxc_l_value.data, (hxc_string){ (const uint8_t *)"melee-sword", 11, true, NULL }.data, hxc_l_value.byte_length) == 0)))
  {
    return hxc_caxecraft_content_RuntimeItemUseProfile_MeleeSword;
  }
  if (!(hxc_l_value.data != NULL && (hxc_l_value.byte_length == (hxc_string){ (const uint8_t *)"place-block", 11, true, NULL }.byte_length && (hxc_l_value.byte_length == 0 || memcmp(hxc_l_value.data, (hxc_string){ (const uint8_t *)"place-block", 11, true, NULL }.data, hxc_l_value.byte_length) == 0))))
  {
    return hxc_caxecraft_content_RuntimeItemUseProfile_NoItemUse;
  }
  return hxc_caxecraft_content_RuntimeItemUseProfile_PlaceBlock;
}

hxc_string hxc_caxecraft_content_RuntimeContentPack_kindOf(struct hxc_array_ref *hxc_l_kinds, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[4] = { (const void *)hxc_l_kinds, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 4, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_returned_string_owner_n4 = { 0 };
  int32_t hxc_l_g = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n0 = hxc_l_g;
    int32_t hxc_l_tmp_array_length_result_n1;
    if (hxc_array_ref_length(hxc_l_kinds, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n0 < hxc_l_tmp_array_length_result_n1))
    {
      break;
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeKindId *hxc_l_tmp_array_get_result_n3;
    if (hxc_array_ref_get_copy(hxc_l_kinds, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_array_get_result_n3;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeKindId *hxc_l_entry = hxc_l_tmp_array_get_result_n3;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeKindId *hxc_l_tmp_load_result_n5 = hxc_l_entry;
    hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_load_result_n5;
    if (hxc_l_tmp_load_result_n5 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n6 = (*hxc_l_tmp_load_result_n5).hxc_id;
    if (hxc_l_tmp_class_field_load_result_n6.data == NULL || hxc_l_id.data == NULL ? hxc_l_tmp_class_field_load_result_n6.data == hxc_l_id.data : hxc_l_tmp_class_field_load_result_n6.byte_length == hxc_l_id.byte_length && (hxc_l_tmp_class_field_load_result_n6.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n6.data, hxc_l_id.data, hxc_l_tmp_class_field_load_result_n6.byte_length) == 0))
    {
      struct hxc_caxecraft_content_RuntimeContentPack_RuntimeKindId *hxc_l_tmp_load_result_n8 = hxc_l_entry;
      hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_load_result_n8;
      if (hxc_l_tmp_load_result_n8 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_class_field_load_result_n9 = (*hxc_l_tmp_load_result_n8).hxc_kind;
      hxc_l_tmp_returned_string_owner_n4 = hxc_l_tmp_class_field_load_result_n9;
      if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n4) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_tmp_returned_string_owner_n4;
    }
  }
  hxc_string hxc_l_tmp_returned_string_owner_n5 = (hxc_string){ (const uint8_t *)"", 0, true, NULL };
  if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_returned_string_owner_n5;
}

struct hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference hxc_caxecraft_content_RuntimeContentPack_optionalReference(struct hxc_caxecraft_content_LocatedOptionalString hxc_l_value)
{
  const void *hxc_l_gc_roots[4] = { NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 4, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (!(hxc_l_value.hxc_value.data == NULL))
  {
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_tmp_managed_class_result_n5 = NULL;
    if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_RuntimeContentPack_RuntimeReference_descriptor, (void **)&hxc_l_tmp_managed_class_result_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_managed_class_result_n5;
    hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeReference(hxc_l_tmp_managed_class_result_n5, hxc_l_value.hxc_value, hxc_l_value.hxc_line, hxc_l_value.hxc_column);
    hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_managed_class_result_n5;
    hxc_l_gc_roots[3] = (struct hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference){ .hxc_tag = hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady, .hxc_payload.hxc_RuntimeReferenceReady.hxc_reference = hxc_l_tmp_managed_class_result_n5 }.hxc_tag == hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady ? (const void *)(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference){ .hxc_tag = hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady, .hxc_payload.hxc_RuntimeReferenceReady.hxc_reference = hxc_l_tmp_managed_class_result_n5 }.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference : NULL;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference){ .hxc_tag = hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady, .hxc_payload.hxc_RuntimeReferenceReady.hxc_reference = hxc_l_tmp_managed_class_result_n5 };
  }
  hxc_l_gc_roots[0] = (struct hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference){ .hxc_tag = hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_NoRuntimeReference }.hxc_tag == hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady ? (const void *)(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference){ .hxc_tag = hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_NoRuntimeReference }.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference : NULL;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference){ .hxc_tag = hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_NoRuntimeReference };
}

struct hxc_caxecraft_content_RuntimePresentation *hxc_caxecraft_content_RuntimeContentPack_readActorPresentation(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, struct hxc_caxecraft_content_ContentJsonNode *hxc_l_node, hxc_string hxc_l_path, struct hxc_caxecraft_content_RuntimeAssetInventory *hxc_l_assets)
{
  const void *hxc_l_gc_roots[9] = { (const void *)hxc_l_node, (const void *)hxc_l_assets, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 9, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n5 = { 0 };
  struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_call_result_n0 = hxc_caxecraft_content_RuntimeContentPack_readPresentation(hxc_l_reader, hxc_l_node, hxc_l_path, hxc_l_assets);
  hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_call_result_n0;
  struct hxc_caxecraft_content_RuntimePresentation *hxc_l_presentation = hxc_l_tmp_call_result_n0;
  hxc_l_gc_roots[3] = (const void *)hxc_l_presentation;
  hxc_l_gc_roots[4] = (const void *)NULL;
  if (hxc_l_presentation == NULL)
  {
    hxc_l_gc_roots[5] = (const void *)NULL;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_load_result_n2 = hxc_l_presentation;
  hxc_l_gc_roots[6] = (const void *)hxc_l_tmp_load_result_n2;
  if (hxc_l_tmp_load_result_n2 == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_class_field_load_result_n3 = (*hxc_l_tmp_load_result_n2).hxc_asset;
  if (!!(hxc_l_tmp_class_field_load_result_n3.data != NULL && (hxc_l_tmp_class_field_load_result_n3.byte_length == (hxc_string){ (const uint8_t *)"entities", 8, true, NULL }.byte_length && (hxc_l_tmp_class_field_load_result_n3.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n3.data, (hxc_string){ (const uint8_t *)"entities", 8, true, NULL }.data, hxc_l_tmp_class_field_load_result_n3.byte_length) == 0))))
  {
    hxc_l_gc_roots[8] = (const void *)hxc_l_presentation;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_presentation;
  }
  if (hxc_l_reader == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_string_concat_result_n5 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".asset", 6, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_instance_call_argument_1_owner_n5 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidInvariant, .hxc_payload.hxc_SchemaInvalidInvariant.hxc_path = hxc_l_tmp_string_concat_result_n5 };
  hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_reader, hxc_l_node, hxc_l_tmp_instance_call_argument_1_owner_n5);
  hxc_l_gc_roots[7] = (const void *)NULL;
  hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n5);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return NULL;
}

struct hxc_array_ref *hxc_caxecraft_content_RuntimeContentPack_readAquaticProfiles(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, struct hxc_caxecraft_content_ContentJsonNode *hxc_l_node)
{
  const void *hxc_l_gc_roots[45] = { (const void *)hxc_l_node, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 45, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_argument_2_owner_n11 = { 0 };
  hxc_string hxc_l_id = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n14 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n16 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n18 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n20 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n22 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n24 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n26 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n28 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n30 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n32 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n34 = { 0 };
  hxc_string hxc_l_path = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n8 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n9 = { 0 };
  if (hxc_l_reader == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeSchemaReader_array(hxc_l_reader, hxc_l_node, (hxc_string){ (const uint8_t *)"aquaticProfiles", 15, true, NULL }, 0, 128);
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_instance_call_result_n0;
  struct hxc_array_ref *hxc_l_values = hxc_l_tmp_instance_call_result_n0;
  hxc_l_gc_roots[2] = (const void *)hxc_l_values;
  hxc_l_gc_roots[3] = (const void *)NULL;
  if (hxc_l_values == NULL)
  {
    hxc_l_gc_roots[4] = (const void *)NULL;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n2 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_array_9ab8d331_descriptor, (void **)&hxc_l_tmp_array_create_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_init_in_place(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition *), _Alignof(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition *), NULL, NULL, NULL, NULL }, hxc_l_tmp_array_create_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_array_create_result_n2;
  struct hxc_array_ref *hxc_l_result = hxc_l_tmp_array_create_result_n2;
  int32_t hxc_l_g = 0;
  hxc_l_gc_roots[6] = (const void *)hxc_l_values;
  int32_t hxc_l_tmp_array_length_result_n4;
  if (hxc_array_ref_length(hxc_l_values, &hxc_l_tmp_array_length_result_n4) != HXC_STATUS_OK)
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
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n7;
    hxc_string hxc_l_tmp_std_string_int_result_n9 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_from_int32(hxc_l_index, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_right_owner_n8 = hxc_l_tmp_std_string_int_result_n9;
    hxc_string hxc_l_tmp_string_concat_result_n11 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"aquaticProfiles[", 16, true, NULL }, hxc_l_tmp_string_concat_right_owner_n8, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n11) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_left_owner_n9 = hxc_l_tmp_string_concat_result_n11;
    hxc_string hxc_l_tmp_string_concat_result_n13 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n9, (hxc_string){ (const uint8_t *)"]", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n13) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_path = hxc_l_tmp_string_concat_result_n13;
    struct hxc_array_ref *hxc_l_tmp_load_result_n14 = hxc_l_values;
    hxc_l_gc_roots[7] = (const void *)hxc_l_tmp_load_result_n14;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_array_get_result_n16;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n14, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n16) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[8] = (const void *)hxc_l_tmp_array_get_result_n16;
    hxc_string hxc_l_tmp_load_result_n17 = hxc_l_path;
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n18 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n18) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"id", 2, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"maximumBreathTicks", 18, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"breathRecoveryPerTick", 21, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"horizontalControlMilli", 22, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"ascentAccelerationMilli", 23, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"descentAccelerationMilli", 24, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"buoyancyAccelerationMilli", 25, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"dragPerTickMilli", 16, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"drowningIntervalTicks", 21, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"underwaterMining", 16, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"coldProtection", 14, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_2_owner_n11 = hxc_l_tmp_array_create_result_n18;
    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n20 = hxc_caxecraft_content_RuntimeSchemaReader_object(hxc_l_reader, hxc_l_tmp_array_get_result_n16, hxc_l_tmp_load_result_n17, hxc_l_tmp_instance_call_argument_2_owner_n11);
    hxc_l_gc_roots[9] = (const void *)hxc_l_tmp_instance_call_result_n20;
    struct hxc_array_ref *hxc_l_fields = hxc_l_tmp_instance_call_result_n20;
    hxc_l_gc_roots[10] = (const void *)hxc_l_fields;
    hxc_l_gc_roots[11] = (const void *)NULL;
    if (hxc_l_fields == NULL)
    {
      hxc_l_gc_roots[13] = (const void *)NULL;
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n11) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n9) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n8) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
    hxc_l_gc_roots[14] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n23 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"id", 2, true, NULL });
    hxc_l_gc_roots[15] = (const void *)hxc_l_tmp_instance_call_result_n23;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_idNode = hxc_l_tmp_instance_call_result_n23;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n24 = hxc_l_idNode;
    hxc_l_gc_roots[16] = (const void *)hxc_l_tmp_load_result_n24;
    hxc_string hxc_l_tmp_string_concat_result_n26 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".id", 3, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n26) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n14 = hxc_l_tmp_string_concat_result_n26;
    hxc_string hxc_l_tmp_call_result_n28 = hxc_caxecraft_content_RuntimeContentPack_readContentId(hxc_l_reader, hxc_l_tmp_load_result_n24, hxc_l_tmp_static_call_argument_2_owner_n14);
    hxc_l_id = hxc_l_tmp_call_result_n28;
    hxc_l_gc_roots[17] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n30 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"maximumBreathTicks", 18, true, NULL });
    hxc_l_gc_roots[18] = (const void *)hxc_l_tmp_instance_call_result_n30;
    hxc_string hxc_l_tmp_string_concat_result_n32 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".maximumBreathTicks", 19, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n32) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n16 = hxc_l_tmp_string_concat_result_n32;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_instance_call_result_n34 = hxc_caxecraft_content_RuntimeSchemaReader_integer(hxc_l_reader, hxc_l_tmp_instance_call_result_n30, hxc_l_tmp_instance_call_argument_1_owner_n16, 1, 12000);
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_maximumBreath = hxc_l_tmp_instance_call_result_n34;
    hxc_l_gc_roots[19] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n36 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"breathRecoveryPerTick", 21, true, NULL });
    hxc_l_gc_roots[20] = (const void *)hxc_l_tmp_instance_call_result_n36;
    hxc_string hxc_l_tmp_string_concat_result_n38 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".breathRecoveryPerTick", 22, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n38) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n18 = hxc_l_tmp_string_concat_result_n38;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_instance_call_result_n40 = hxc_caxecraft_content_RuntimeSchemaReader_integer(hxc_l_reader, hxc_l_tmp_instance_call_result_n36, hxc_l_tmp_instance_call_argument_1_owner_n18, 1, 120);
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_recovery = hxc_l_tmp_instance_call_result_n40;
    hxc_l_gc_roots[21] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n42 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"horizontalControlMilli", 22, true, NULL });
    hxc_l_gc_roots[22] = (const void *)hxc_l_tmp_instance_call_result_n42;
    hxc_string hxc_l_tmp_string_concat_result_n44 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".horizontalControlMilli", 23, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n44) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n20 = hxc_l_tmp_string_concat_result_n44;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_instance_call_result_n46 = hxc_caxecraft_content_RuntimeSchemaReader_integer(hxc_l_reader, hxc_l_tmp_instance_call_result_n42, hxc_l_tmp_instance_call_argument_1_owner_n20, 0, 1000);
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_horizontal = hxc_l_tmp_instance_call_result_n46;
    hxc_l_gc_roots[23] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n48 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"ascentAccelerationMilli", 23, true, NULL });
    hxc_l_gc_roots[24] = (const void *)hxc_l_tmp_instance_call_result_n48;
    hxc_string hxc_l_tmp_string_concat_result_n50 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".ascentAccelerationMilli", 24, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n50) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n22 = hxc_l_tmp_string_concat_result_n50;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_instance_call_result_n52 = hxc_caxecraft_content_RuntimeSchemaReader_integer(hxc_l_reader, hxc_l_tmp_instance_call_result_n48, hxc_l_tmp_instance_call_argument_1_owner_n22, 0, 40000);
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_ascent = hxc_l_tmp_instance_call_result_n52;
    hxc_l_gc_roots[25] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n54 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"descentAccelerationMilli", 24, true, NULL });
    hxc_l_gc_roots[26] = (const void *)hxc_l_tmp_instance_call_result_n54;
    hxc_string hxc_l_tmp_string_concat_result_n56 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".descentAccelerationMilli", 25, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n56) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n24 = hxc_l_tmp_string_concat_result_n56;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_instance_call_result_n58 = hxc_caxecraft_content_RuntimeSchemaReader_integer(hxc_l_reader, hxc_l_tmp_instance_call_result_n54, hxc_l_tmp_instance_call_argument_1_owner_n24, 0, 40000);
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_descent = hxc_l_tmp_instance_call_result_n58;
    hxc_l_gc_roots[27] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n60 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"buoyancyAccelerationMilli", 25, true, NULL });
    hxc_l_gc_roots[28] = (const void *)hxc_l_tmp_instance_call_result_n60;
    hxc_string hxc_l_tmp_string_concat_result_n62 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".buoyancyAccelerationMilli", 26, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n62) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n26 = hxc_l_tmp_string_concat_result_n62;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_instance_call_result_n64 = hxc_caxecraft_content_RuntimeSchemaReader_integer(hxc_l_reader, hxc_l_tmp_instance_call_result_n60, hxc_l_tmp_instance_call_argument_1_owner_n26, 0, 30000);
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_buoyancy = hxc_l_tmp_instance_call_result_n64;
    hxc_l_gc_roots[29] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n66 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"dragPerTickMilli", 16, true, NULL });
    hxc_l_gc_roots[30] = (const void *)hxc_l_tmp_instance_call_result_n66;
    hxc_string hxc_l_tmp_string_concat_result_n68 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".dragPerTickMilli", 17, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n68) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n28 = hxc_l_tmp_string_concat_result_n68;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_instance_call_result_n70 = hxc_caxecraft_content_RuntimeSchemaReader_integer(hxc_l_reader, hxc_l_tmp_instance_call_result_n66, hxc_l_tmp_instance_call_argument_1_owner_n28, 0, 900);
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_drag = hxc_l_tmp_instance_call_result_n70;
    hxc_l_gc_roots[31] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n72 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"drowningIntervalTicks", 21, true, NULL });
    hxc_l_gc_roots[32] = (const void *)hxc_l_tmp_instance_call_result_n72;
    hxc_string hxc_l_tmp_string_concat_result_n74 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".drowningIntervalTicks", 22, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n74) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n30 = hxc_l_tmp_string_concat_result_n74;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_instance_call_result_n76 = hxc_caxecraft_content_RuntimeSchemaReader_integer(hxc_l_reader, hxc_l_tmp_instance_call_result_n72, hxc_l_tmp_instance_call_argument_1_owner_n30, 1, 1200);
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_drowning = hxc_l_tmp_instance_call_result_n76;
    hxc_l_gc_roots[33] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n78 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"underwaterMining", 16, true, NULL });
    hxc_l_gc_roots[34] = (const void *)hxc_l_tmp_instance_call_result_n78;
    hxc_string hxc_l_tmp_string_concat_result_n80 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".underwaterMining", 17, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n80) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n32 = hxc_l_tmp_string_concat_result_n80;
    struct hxc_optional_value_hc2f19e60157e hxc_l_tmp_instance_call_result_n82 = hxc_caxecraft_content_RuntimeSchemaReader_boolean(hxc_l_reader, hxc_l_tmp_instance_call_result_n78, hxc_l_tmp_instance_call_argument_1_owner_n32);
    struct hxc_optional_value_hc2f19e60157e hxc_l_mining = hxc_l_tmp_instance_call_result_n82;
    hxc_l_gc_roots[35] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n84 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"coldProtection", 14, true, NULL });
    hxc_l_gc_roots[36] = (const void *)hxc_l_tmp_instance_call_result_n84;
    hxc_string hxc_l_tmp_string_concat_result_n86 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".coldProtection", 15, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n86) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n34 = hxc_l_tmp_string_concat_result_n86;
    struct hxc_optional_value_hc2f19e60157e hxc_l_tmp_instance_call_result_n88 = hxc_caxecraft_content_RuntimeSchemaReader_boolean(hxc_l_reader, hxc_l_tmp_instance_call_result_n84, hxc_l_tmp_instance_call_argument_1_owner_n34);
    struct hxc_optional_value_hc2f19e60157e hxc_l_cold = hxc_l_tmp_instance_call_result_n88;
    struct hxc_optional_caxecraft_content_RuntimeSchemaDiagnostic hxc_l_tmp_class_field_load_result_n89 = (*hxc_l_reader).hxc_failure;
    bool hxc_l_tmp_short_circuit_result_n35 = hxc_l_tmp_class_field_load_result_n89.hxc_has_value;
    if (!hxc_l_tmp_class_field_load_result_n89.hxc_has_value)
    {
      hxc_l_tmp_short_circuit_result_n35 = hxc_l_id.data == NULL;
    }
    bool hxc_l_tmp_short_circuit_load_result_n91 = hxc_l_tmp_short_circuit_result_n35;
    bool hxc_l_tmp_short_circuit_result_n36 = hxc_l_tmp_short_circuit_load_result_n91;
    if (!hxc_l_tmp_short_circuit_load_result_n91)
    {
      hxc_l_tmp_short_circuit_result_n36 = !hxc_l_maximumBreath.hxc_has_value;
    }
    bool hxc_l_tmp_short_circuit_load_result_n93 = hxc_l_tmp_short_circuit_result_n36;
    bool hxc_l_tmp_short_circuit_result_n37 = hxc_l_tmp_short_circuit_load_result_n93;
    if (!hxc_l_tmp_short_circuit_load_result_n93)
    {
      hxc_l_tmp_short_circuit_result_n37 = !hxc_l_recovery.hxc_has_value;
    }
    bool hxc_l_tmp_short_circuit_load_result_n95 = hxc_l_tmp_short_circuit_result_n37;
    bool hxc_l_tmp_short_circuit_result_n38 = hxc_l_tmp_short_circuit_load_result_n95;
    if (!hxc_l_tmp_short_circuit_load_result_n95)
    {
      hxc_l_tmp_short_circuit_result_n38 = !hxc_l_horizontal.hxc_has_value;
    }
    bool hxc_l_tmp_short_circuit_load_result_n97 = hxc_l_tmp_short_circuit_result_n38;
    bool hxc_l_tmp_short_circuit_result_n39 = hxc_l_tmp_short_circuit_load_result_n97;
    if (!hxc_l_tmp_short_circuit_load_result_n97)
    {
      hxc_l_tmp_short_circuit_result_n39 = !hxc_l_ascent.hxc_has_value;
    }
    bool hxc_l_tmp_short_circuit_load_result_n99 = hxc_l_tmp_short_circuit_result_n39;
    bool hxc_l_tmp_short_circuit_result_n40 = hxc_l_tmp_short_circuit_load_result_n99;
    if (!hxc_l_tmp_short_circuit_load_result_n99)
    {
      hxc_l_tmp_short_circuit_result_n40 = !hxc_l_descent.hxc_has_value;
    }
    bool hxc_l_tmp_short_circuit_load_result_n101 = hxc_l_tmp_short_circuit_result_n40;
    bool hxc_l_tmp_short_circuit_result_n41 = hxc_l_tmp_short_circuit_load_result_n101;
    if (!hxc_l_tmp_short_circuit_load_result_n101)
    {
      hxc_l_tmp_short_circuit_result_n41 = !hxc_l_buoyancy.hxc_has_value;
    }
    bool hxc_l_tmp_short_circuit_load_result_n103 = hxc_l_tmp_short_circuit_result_n41;
    bool hxc_l_tmp_short_circuit_result_n42 = hxc_l_tmp_short_circuit_load_result_n103;
    if (!hxc_l_tmp_short_circuit_load_result_n103)
    {
      hxc_l_tmp_short_circuit_result_n42 = !hxc_l_drag.hxc_has_value;
    }
    bool hxc_l_tmp_short_circuit_load_result_n105 = hxc_l_tmp_short_circuit_result_n42;
    bool hxc_l_tmp_short_circuit_result_n43 = hxc_l_tmp_short_circuit_load_result_n105;
    if (!hxc_l_tmp_short_circuit_load_result_n105)
    {
      hxc_l_tmp_short_circuit_result_n43 = !hxc_l_drowning.hxc_has_value;
    }
    bool hxc_l_tmp_short_circuit_load_result_n107 = hxc_l_tmp_short_circuit_result_n43;
    bool hxc_l_tmp_short_circuit_result_n44 = hxc_l_tmp_short_circuit_load_result_n107;
    if (!hxc_l_tmp_short_circuit_load_result_n107)
    {
      hxc_l_tmp_short_circuit_result_n44 = !hxc_l_mining.hxc_has_value;
    }
    bool hxc_l_tmp_short_circuit_load_result_n109 = hxc_l_tmp_short_circuit_result_n44;
    bool hxc_l_tmp_short_circuit_result_n45 = hxc_l_tmp_short_circuit_load_result_n109;
    if (!hxc_l_tmp_short_circuit_load_result_n109)
    {
      hxc_l_tmp_short_circuit_result_n45 = !hxc_l_cold.hxc_has_value;
    }
    if (hxc_l_tmp_short_circuit_result_n45)
    {
      hxc_l_gc_roots[37] = (const void *)NULL;
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n34) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n32) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n30) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n28) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n26) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n24) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n22) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n20) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n18) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n16) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n14) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n11) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n9) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n8) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n112 = hxc_l_result;
    hxc_l_gc_roots[38] = (const void *)hxc_l_tmp_load_result_n112;
    hxc_string hxc_l_tmp_load_result_n113 = hxc_l_id;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n114 = hxc_l_idNode;
    hxc_l_gc_roots[39] = (const void *)hxc_l_tmp_load_result_n114;
    if (hxc_l_tmp_load_result_n114 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n115 = (*hxc_l_tmp_load_result_n114).hxc_line;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n116 = hxc_l_idNode;
    hxc_l_gc_roots[40] = (const void *)hxc_l_tmp_load_result_n116;
    if (hxc_l_tmp_load_result_n116 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n117 = (*hxc_l_tmp_load_result_n116).hxc_column;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n118 = hxc_l_maximumBreath;
    if (!hxc_l_tmp_load_result_n118.hxc_has_value)
    {
      abort();
    }
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n120 = hxc_l_recovery;
    if (!hxc_l_tmp_load_result_n120.hxc_has_value)
    {
      abort();
    }
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n122 = hxc_l_horizontal;
    if (!hxc_l_tmp_load_result_n122.hxc_has_value)
    {
      abort();
    }
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n124 = hxc_l_ascent;
    if (!hxc_l_tmp_load_result_n124.hxc_has_value)
    {
      abort();
    }
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n126 = hxc_l_descent;
    if (!hxc_l_tmp_load_result_n126.hxc_has_value)
    {
      abort();
    }
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n128 = hxc_l_buoyancy;
    if (!hxc_l_tmp_load_result_n128.hxc_has_value)
    {
      abort();
    }
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n130 = hxc_l_drag;
    if (!hxc_l_tmp_load_result_n130.hxc_has_value)
    {
      abort();
    }
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n132 = hxc_l_drowning;
    if (!hxc_l_tmp_load_result_n132.hxc_has_value)
    {
      abort();
    }
    struct hxc_optional_value_hc2f19e60157e hxc_l_tmp_load_result_n134 = hxc_l_mining;
    if (!hxc_l_tmp_load_result_n134.hxc_has_value)
    {
      abort();
    }
    struct hxc_optional_value_hc2f19e60157e hxc_l_tmp_load_result_n136 = hxc_l_cold;
    if (!hxc_l_tmp_load_result_n136.hxc_has_value)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition *hxc_l_tmp_managed_class_result_n138 = NULL;
    if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition_descriptor, (void **)&hxc_l_tmp_managed_class_result_n138) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[41] = (const void *)hxc_l_tmp_managed_class_result_n138;
    hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition(hxc_l_tmp_managed_class_result_n138, hxc_l_tmp_load_result_n113, hxc_l_tmp_class_field_load_result_n115, hxc_l_tmp_class_field_load_result_n117, hxc_l_tmp_load_result_n118.hxc_value, hxc_l_tmp_load_result_n120.hxc_value, hxc_l_tmp_load_result_n122.hxc_value, hxc_l_tmp_load_result_n124.hxc_value, hxc_l_tmp_load_result_n126.hxc_value, hxc_l_tmp_load_result_n128.hxc_value, hxc_l_tmp_load_result_n130.hxc_value, hxc_l_tmp_load_result_n132.hxc_value, hxc_l_tmp_load_result_n134.hxc_value, hxc_l_tmp_load_result_n136.hxc_value);
    hxc_l_gc_roots[42] = (const void *)hxc_l_tmp_managed_class_result_n138;
    int32_t hxc_l_tmp_array_push_result_n139;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n112, &(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition *){ hxc_l_tmp_managed_class_result_n138 }, &hxc_l_tmp_array_push_result_n139) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n139;
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n34) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n32) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n30) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n28) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n26) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n24) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n22) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n20) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n18) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n16) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n14) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n11) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n8) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  hxc_l_gc_roots[12] = (const void *)hxc_l_result;
  bool hxc_l_tmp_call_result_n141 = hxc_caxecraft_content_RuntimeContentPack_validateAquaticOrder(hxc_l_reader, (hxc_string){ (const uint8_t *)"aquaticProfiles", 15, true, NULL }, hxc_l_result);
  if (!hxc_l_tmp_call_result_n141)
  {
    hxc_l_gc_roots[44] = (const void *)NULL;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  hxc_l_gc_roots[43] = (const void *)hxc_l_result;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_result;
}

struct hxc_array_ref *hxc_caxecraft_content_RuntimeContentPack_readBlocks(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, struct hxc_caxecraft_content_ContentJsonNode *hxc_l_node)
{
  const void *hxc_l_gc_roots[39] = { (const void *)hxc_l_node, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 39, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_argument_2_owner_n11 = { 0 };
  hxc_string hxc_l_id = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n14 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n16 = { 0 };
  hxc_string hxc_l_collision = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n18 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_static_call_argument_3_owner_n19 = { 0 };
  hxc_string hxc_l_edit = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n21 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_static_call_argument_3_owner_n22 = { 0 };
  struct hxc_optional_caxecraft_content_LocatedOptionalString hxc_l_dropItem = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n24 = { 0 };
  hxc_string hxc_l_renderProfile = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n26 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_static_call_argument_3_owner_n27 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n34 = { 0 };
  hxc_string hxc_l_path = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n8 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n9 = { 0 };
  if (hxc_l_reader == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeSchemaReader_array(hxc_l_reader, hxc_l_node, (hxc_string){ (const uint8_t *)"blocks", 6, true, NULL }, 0, 128);
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_instance_call_result_n0;
  struct hxc_array_ref *hxc_l_values = hxc_l_tmp_instance_call_result_n0;
  hxc_l_gc_roots[2] = (const void *)hxc_l_values;
  hxc_l_gc_roots[3] = (const void *)NULL;
  if (hxc_l_values == NULL)
  {
    hxc_l_gc_roots[4] = (const void *)NULL;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n2 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_array_87e9c36e_descriptor, (void **)&hxc_l_tmp_array_create_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_init_in_place(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *), _Alignof(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *), NULL, NULL, NULL, NULL }, hxc_l_tmp_array_create_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_array_create_result_n2;
  struct hxc_array_ref *hxc_l_result = hxc_l_tmp_array_create_result_n2;
  int32_t hxc_l_g = 0;
  hxc_l_gc_roots[6] = (const void *)hxc_l_values;
  int32_t hxc_l_tmp_array_length_result_n4;
  if (hxc_array_ref_length(hxc_l_values, &hxc_l_tmp_array_length_result_n4) != HXC_STATUS_OK)
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
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n7;
    hxc_string hxc_l_tmp_std_string_int_result_n9 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_from_int32(hxc_l_index, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_right_owner_n8 = hxc_l_tmp_std_string_int_result_n9;
    hxc_string hxc_l_tmp_string_concat_result_n11 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"blocks[", 7, true, NULL }, hxc_l_tmp_string_concat_right_owner_n8, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n11) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_left_owner_n9 = hxc_l_tmp_string_concat_result_n11;
    hxc_string hxc_l_tmp_string_concat_result_n13 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n9, (hxc_string){ (const uint8_t *)"]", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n13) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_path = hxc_l_tmp_string_concat_result_n13;
    struct hxc_array_ref *hxc_l_tmp_load_result_n14 = hxc_l_values;
    hxc_l_gc_roots[7] = (const void *)hxc_l_tmp_load_result_n14;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_array_get_result_n16;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n14, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n16) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[8] = (const void *)hxc_l_tmp_array_get_result_n16;
    hxc_string hxc_l_tmp_load_result_n17 = hxc_l_path;
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n18 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n18) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"id", 2, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"storageCode", 11, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"collision", 9, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"edit", 4, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"dropItem", 8, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"renderProfile", 13, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_2_owner_n11 = hxc_l_tmp_array_create_result_n18;
    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n20 = hxc_caxecraft_content_RuntimeSchemaReader_object(hxc_l_reader, hxc_l_tmp_array_get_result_n16, hxc_l_tmp_load_result_n17, hxc_l_tmp_instance_call_argument_2_owner_n11);
    hxc_l_gc_roots[9] = (const void *)hxc_l_tmp_instance_call_result_n20;
    struct hxc_array_ref *hxc_l_fields = hxc_l_tmp_instance_call_result_n20;
    hxc_l_gc_roots[10] = (const void *)hxc_l_fields;
    hxc_l_gc_roots[11] = (const void *)NULL;
    if (hxc_l_fields == NULL)
    {
      hxc_l_gc_roots[13] = (const void *)NULL;
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n11) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n9) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n8) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
    hxc_l_gc_roots[14] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n23 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"id", 2, true, NULL });
    hxc_l_gc_roots[15] = (const void *)hxc_l_tmp_instance_call_result_n23;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_idNode = hxc_l_tmp_instance_call_result_n23;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n24 = hxc_l_idNode;
    hxc_l_gc_roots[16] = (const void *)hxc_l_tmp_load_result_n24;
    hxc_string hxc_l_tmp_string_concat_result_n26 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".id", 3, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n26) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n14 = hxc_l_tmp_string_concat_result_n26;
    hxc_string hxc_l_tmp_call_result_n28 = hxc_caxecraft_content_RuntimeContentPack_readContentId(hxc_l_reader, hxc_l_tmp_load_result_n24, hxc_l_tmp_static_call_argument_2_owner_n14);
    hxc_l_id = hxc_l_tmp_call_result_n28;
    hxc_l_gc_roots[17] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n30 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"storageCode", 11, true, NULL });
    hxc_l_gc_roots[18] = (const void *)hxc_l_tmp_instance_call_result_n30;
    hxc_string hxc_l_tmp_string_concat_result_n32 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".storageCode", 12, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n32) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n16 = hxc_l_tmp_string_concat_result_n32;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_instance_call_result_n34 = hxc_caxecraft_content_RuntimeSchemaReader_integer(hxc_l_reader, hxc_l_tmp_instance_call_result_n30, hxc_l_tmp_instance_call_argument_1_owner_n16, 0, 255);
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_storage = hxc_l_tmp_instance_call_result_n34;
    hxc_l_gc_roots[19] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n36 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"collision", 9, true, NULL });
    hxc_l_gc_roots[20] = (const void *)hxc_l_tmp_instance_call_result_n36;
    hxc_string hxc_l_tmp_string_concat_result_n38 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".collision", 10, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n38) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n18 = hxc_l_tmp_string_concat_result_n38;
    hxc_string hxc_l_tmp_static_call_argument_2_borrow_result_n39 = hxc_l_tmp_static_call_argument_2_owner_n18;
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n40 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n40) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n40->value, &(hxc_string){ (const uint8_t *)"passable", 8, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n40->value, &(hxc_string){ (const uint8_t *)"solid", 5, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_3_owner_n19 = hxc_l_tmp_array_create_result_n40;
    hxc_string hxc_l_tmp_call_result_n42 = hxc_caxecraft_content_RuntimeContentPack_readClosed(hxc_l_reader, hxc_l_tmp_instance_call_result_n36, hxc_l_tmp_static_call_argument_2_borrow_result_n39, hxc_l_tmp_static_call_argument_3_owner_n19);
    hxc_l_collision = hxc_l_tmp_call_result_n42;
    hxc_l_gc_roots[21] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n44 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"edit", 4, true, NULL });
    hxc_l_gc_roots[22] = (const void *)hxc_l_tmp_instance_call_result_n44;
    hxc_string hxc_l_tmp_string_concat_result_n46 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".edit", 5, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n46) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n21 = hxc_l_tmp_string_concat_result_n46;
    hxc_string hxc_l_tmp_static_call_argument_2_borrow_result_n47 = hxc_l_tmp_static_call_argument_2_owner_n21;
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n48 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n48) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n48->value, &(hxc_string){ (const uint8_t *)"collectable", 11, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n48->value, &(hxc_string){ (const uint8_t *)"immutable", 9, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_3_owner_n22 = hxc_l_tmp_array_create_result_n48;
    hxc_string hxc_l_tmp_call_result_n50 = hxc_caxecraft_content_RuntimeContentPack_readClosed(hxc_l_reader, hxc_l_tmp_instance_call_result_n44, hxc_l_tmp_static_call_argument_2_borrow_result_n47, hxc_l_tmp_static_call_argument_3_owner_n22);
    hxc_l_edit = hxc_l_tmp_call_result_n50;
    hxc_l_gc_roots[23] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n52 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"dropItem", 8, true, NULL });
    hxc_l_gc_roots[24] = (const void *)hxc_l_tmp_instance_call_result_n52;
    hxc_string hxc_l_tmp_string_concat_result_n54 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".dropItem", 9, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n54) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n24 = hxc_l_tmp_string_concat_result_n54;
    struct hxc_optional_caxecraft_content_LocatedOptionalString hxc_l_tmp_instance_call_result_n56 = hxc_caxecraft_content_RuntimeSchemaReader_nullableContentId(hxc_l_reader, hxc_l_tmp_instance_call_result_n52, hxc_l_tmp_instance_call_argument_1_owner_n24);
    hxc_l_dropItem = hxc_l_tmp_instance_call_result_n56;
    hxc_l_gc_roots[25] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n58 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"renderProfile", 13, true, NULL });
    hxc_l_gc_roots[26] = (const void *)hxc_l_tmp_instance_call_result_n58;
    hxc_string hxc_l_tmp_string_concat_result_n60 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".renderProfile", 14, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n60) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n26 = hxc_l_tmp_string_concat_result_n60;
    hxc_string hxc_l_tmp_static_call_argument_2_borrow_result_n61 = hxc_l_tmp_static_call_argument_2_owner_n26;
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n62 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n62) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n62->value, &(hxc_string){ (const uint8_t *)"air", 3, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n62->value, &(hxc_string){ (const uint8_t *)"ash-field", 9, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n62->value, &(hxc_string){ (const uint8_t *)"foundation-rock", 15, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n62->value, &(hxc_string){ (const uint8_t *)"forest-leaves", 13, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n62->value, &(hxc_string){ (const uint8_t *)"forest-wood", 11, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n62->value, &(hxc_string){ (const uint8_t *)"meadow-grass", 12, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n62->value, &(hxc_string){ (const uint8_t *)"rich-soil", 9, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n62->value, &(hxc_string){ (const uint8_t *)"river-sand", 10, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n62->value, &(hxc_string){ (const uint8_t *)"slate-stone", 11, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n62->value, &(hxc_string){ (const uint8_t *)"snow-field", 10, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_3_owner_n27 = hxc_l_tmp_array_create_result_n62;
    hxc_string hxc_l_tmp_call_result_n64 = hxc_caxecraft_content_RuntimeContentPack_readClosed(hxc_l_reader, hxc_l_tmp_instance_call_result_n58, hxc_l_tmp_static_call_argument_2_borrow_result_n61, hxc_l_tmp_static_call_argument_3_owner_n27);
    hxc_l_renderProfile = hxc_l_tmp_call_result_n64;
    struct hxc_optional_caxecraft_content_RuntimeSchemaDiagnostic hxc_l_tmp_class_field_load_result_n65 = (*hxc_l_reader).hxc_failure;
    bool hxc_l_tmp_short_circuit_result_n28 = hxc_l_tmp_class_field_load_result_n65.hxc_has_value;
    if (!hxc_l_tmp_class_field_load_result_n65.hxc_has_value)
    {
      hxc_l_tmp_short_circuit_result_n28 = hxc_l_id.data == NULL;
    }
    bool hxc_l_tmp_short_circuit_load_result_n67 = hxc_l_tmp_short_circuit_result_n28;
    bool hxc_l_tmp_short_circuit_result_n29 = hxc_l_tmp_short_circuit_load_result_n67;
    if (!hxc_l_tmp_short_circuit_load_result_n67)
    {
      hxc_l_tmp_short_circuit_result_n29 = !hxc_l_storage.hxc_has_value;
    }
    bool hxc_l_tmp_short_circuit_load_result_n69 = hxc_l_tmp_short_circuit_result_n29;
    bool hxc_l_tmp_short_circuit_result_n30 = hxc_l_tmp_short_circuit_load_result_n69;
    if (!hxc_l_tmp_short_circuit_load_result_n69)
    {
      hxc_l_tmp_short_circuit_result_n30 = hxc_l_collision.data == NULL;
    }
    bool hxc_l_tmp_short_circuit_load_result_n71 = hxc_l_tmp_short_circuit_result_n30;
    bool hxc_l_tmp_short_circuit_result_n31 = hxc_l_tmp_short_circuit_load_result_n71;
    if (!hxc_l_tmp_short_circuit_load_result_n71)
    {
      hxc_l_tmp_short_circuit_result_n31 = hxc_l_edit.data == NULL;
    }
    bool hxc_l_tmp_short_circuit_load_result_n73 = hxc_l_tmp_short_circuit_result_n31;
    bool hxc_l_tmp_short_circuit_result_n32 = hxc_l_tmp_short_circuit_load_result_n73;
    if (!hxc_l_tmp_short_circuit_load_result_n73)
    {
      hxc_l_tmp_short_circuit_result_n32 = !hxc_l_dropItem.hxc_has_value;
    }
    bool hxc_l_tmp_short_circuit_load_result_n75 = hxc_l_tmp_short_circuit_result_n32;
    bool hxc_l_tmp_short_circuit_result_n33 = hxc_l_tmp_short_circuit_load_result_n75;
    if (!hxc_l_tmp_short_circuit_load_result_n75)
    {
      hxc_l_tmp_short_circuit_result_n33 = hxc_l_renderProfile.data == NULL;
    }
    if (hxc_l_tmp_short_circuit_result_n33)
    {
      hxc_l_gc_roots[27] = (const void *)NULL;
      if (hxc_string_release(&hxc_l_renderProfile) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n27) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n26) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_optional_01d9fa01_destroy(&hxc_l_dropItem);
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n24) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_edit) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n22) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n21) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_collision) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n19) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n18) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n16) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n14) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n11) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n9) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n8) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
    hxc_string hxc_l_tmp_load_result_n78 = hxc_l_edit;
    struct hxc_optional_caxecraft_content_LocatedOptionalString hxc_l_tmp_load_result_n80 = hxc_l_dropItem;
    if (!hxc_l_tmp_load_result_n80.hxc_has_value)
    {
      abort();
    }
    if ((hxc_l_tmp_load_result_n78.data != NULL && (hxc_l_tmp_load_result_n78.byte_length == (hxc_string){ (const uint8_t *)"collectable", 11, true, NULL }.byte_length && (hxc_l_tmp_load_result_n78.byte_length == 0 || memcmp(hxc_l_tmp_load_result_n78.data, (hxc_string){ (const uint8_t *)"collectable", 11, true, NULL }.data, hxc_l_tmp_load_result_n78.byte_length) == 0))) != (hxc_l_tmp_load_result_n80.hxc_value.hxc_value.data != NULL))
    {
      struct hxc_array_ref *hxc_l_tmp_load_result_n83 = hxc_l_values;
      hxc_l_gc_roots[28] = (const void *)hxc_l_tmp_load_result_n83;
      struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_array_get_result_n85;
      if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n83, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n85) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_gc_roots[29] = (const void *)hxc_l_tmp_array_get_result_n85;
      hxc_string hxc_l_tmp_string_concat_result_n87 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".dropItem", 9, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n87) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_instance_call_argument_1_owner_n34 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidInvariant, .hxc_payload.hxc_SchemaInvalidInvariant.hxc_path = hxc_l_tmp_string_concat_result_n87 };
      hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_reader, hxc_l_tmp_array_get_result_n85, hxc_l_tmp_instance_call_argument_1_owner_n34);
      hxc_l_gc_roots[30] = (const void *)NULL;
      hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n34);
      if (hxc_string_release(&hxc_l_renderProfile) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n27) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n26) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_optional_01d9fa01_destroy(&hxc_l_dropItem);
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n24) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_edit) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n22) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n21) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_collision) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n19) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n18) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n16) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n14) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n11) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n9) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n8) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n90 = hxc_l_result;
    hxc_l_gc_roots[31] = (const void *)hxc_l_tmp_load_result_n90;
    hxc_string hxc_l_tmp_load_result_n91 = hxc_l_id;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n92 = hxc_l_idNode;
    hxc_l_gc_roots[32] = (const void *)hxc_l_tmp_load_result_n92;
    if (hxc_l_tmp_load_result_n92 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n93 = (*hxc_l_tmp_load_result_n92).hxc_line;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n94 = hxc_l_idNode;
    hxc_l_gc_roots[33] = (const void *)hxc_l_tmp_load_result_n94;
    if (hxc_l_tmp_load_result_n94 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n95 = (*hxc_l_tmp_load_result_n94).hxc_column;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n96 = hxc_l_storage;
    if (!hxc_l_tmp_load_result_n96.hxc_has_value)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n98 = hxc_l_collision;
    hxc_string hxc_l_tmp_load_result_n100 = hxc_l_edit;
    struct hxc_optional_caxecraft_content_LocatedOptionalString hxc_l_tmp_load_result_n102 = hxc_l_dropItem;
    if (!hxc_l_tmp_load_result_n102.hxc_has_value)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference hxc_l_tmp_call_result_n104 = hxc_caxecraft_content_RuntimeContentPack_optionalReference(hxc_l_tmp_load_result_n102.hxc_value);
    hxc_l_gc_roots[34] = hxc_l_tmp_call_result_n104.hxc_tag == hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady ? (const void *)hxc_l_tmp_call_result_n104.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference : NULL;
    hxc_string hxc_l_tmp_load_result_n105 = hxc_l_renderProfile;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_managed_class_result_n106 = NULL;
    if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition_descriptor, (void **)&hxc_l_tmp_managed_class_result_n106) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[35] = (const void *)hxc_l_tmp_managed_class_result_n106;
    hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition(hxc_l_tmp_managed_class_result_n106, hxc_l_tmp_load_result_n91, hxc_l_tmp_class_field_load_result_n93, hxc_l_tmp_class_field_load_result_n95, hxc_l_tmp_load_result_n96.hxc_value, hxc_l_tmp_load_result_n98.data != NULL && (hxc_l_tmp_load_result_n98.byte_length == (hxc_string){ (const uint8_t *)"passable", 8, true, NULL }.byte_length && (hxc_l_tmp_load_result_n98.byte_length == 0 || memcmp(hxc_l_tmp_load_result_n98.data, (hxc_string){ (const uint8_t *)"passable", 8, true, NULL }.data, hxc_l_tmp_load_result_n98.byte_length) == 0)), hxc_l_tmp_load_result_n100.data != NULL && (hxc_l_tmp_load_result_n100.byte_length == (hxc_string){ (const uint8_t *)"collectable", 11, true, NULL }.byte_length && (hxc_l_tmp_load_result_n100.byte_length == 0 || memcmp(hxc_l_tmp_load_result_n100.data, (hxc_string){ (const uint8_t *)"collectable", 11, true, NULL }.data, hxc_l_tmp_load_result_n100.byte_length) == 0)), hxc_l_tmp_call_result_n104, hxc_l_tmp_load_result_n105);
    hxc_l_gc_roots[36] = (const void *)hxc_l_tmp_managed_class_result_n106;
    int32_t hxc_l_tmp_array_push_result_n107;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n90, &(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *){ hxc_l_tmp_managed_class_result_n106 }, &hxc_l_tmp_array_push_result_n107) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n107;
    if (hxc_string_release(&hxc_l_renderProfile) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n27) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n26) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_01d9fa01_destroy(&hxc_l_dropItem);
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n24) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_edit) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n22) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n21) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_collision) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n19) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n18) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n16) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n14) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n11) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n8) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  hxc_l_gc_roots[12] = (const void *)hxc_l_result;
  bool hxc_l_tmp_call_result_n109 = hxc_caxecraft_content_RuntimeContentPack_validateBlockOrder(hxc_l_reader, (hxc_string){ (const uint8_t *)"blocks", 6, true, NULL }, hxc_l_result);
  if (!hxc_l_tmp_call_result_n109)
  {
    hxc_l_gc_roots[38] = (const void *)NULL;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  hxc_l_gc_roots[37] = (const void *)hxc_l_result;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_result;
}

hxc_string hxc_caxecraft_content_RuntimeContentPack_readClosed(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, struct hxc_caxecraft_content_ContentJsonNode *hxc_l_node, hxc_string hxc_l_path, struct hxc_array_ref *hxc_l_admitted)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_node };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n13 = { 0 };
  hxc_string hxc_l_tmp_returned_string_owner_n5 = { 0 };
  hxc_string hxc_l_choice = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n9 = { 0 };
  if (hxc_l_reader == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeSchemaReader_string(hxc_l_reader, hxc_l_node, hxc_l_path, 128);
  hxc_string hxc_l_value = hxc_l_tmp_instance_call_result_n0;
  if (hxc_l_value.data == NULL)
  {
    hxc_l_tmp_returned_string_owner_n5 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_string hxc_l_tmp_returned_string_owned_load_result_n2 = hxc_l_tmp_returned_string_owner_n5;
    if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_returned_string_owned_load_result_n2;
  }
  bool hxc_l_found = false;
  int32_t hxc_l_g = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n3 = hxc_l_g;
    int32_t hxc_l_tmp_array_length_result_n4;
    if (hxc_array_ref_length(hxc_l_admitted, &hxc_l_tmp_array_length_result_n4) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n3 < hxc_l_tmp_array_length_result_n4))
    {
      break;
    }
    hxc_string hxc_l_tmp_array_get_result_n6;
    if (hxc_array_ref_get_copy(hxc_l_admitted, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n9 = hxc_l_tmp_array_get_result_n6;
    hxc_l_choice = hxc_l_tmp_array_element_owner_n9;
    if (hxc_string_retain(hxc_l_choice) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
    hxc_string hxc_l_tmp_load_result_n9 = hxc_l_choice;
    if (hxc_l_tmp_load_result_n9.data == NULL || hxc_l_value.data == NULL ? hxc_l_tmp_load_result_n9.data == hxc_l_value.data : hxc_l_tmp_load_result_n9.byte_length == hxc_l_value.byte_length && (hxc_l_tmp_load_result_n9.byte_length == 0 || memcmp(hxc_l_tmp_load_result_n9.data, hxc_l_value.data, hxc_l_tmp_load_result_n9.byte_length) == 0))
    {
      hxc_l_found = true;
    }
    if (hxc_string_release(&hxc_l_choice) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_array_element_owner_n9) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  bool hxc_l_tmp_call_result_n13 = hxc_caxecraft_content_RuntimeSchemaReader_validProfile(hxc_l_value);
  bool hxc_l_tmp_short_circuit_result_n10 = !hxc_l_tmp_call_result_n13;
  if (!!hxc_l_tmp_call_result_n13)
  {
    hxc_l_tmp_short_circuit_result_n10 = !hxc_l_found;
  }
  if (!hxc_l_tmp_short_circuit_result_n10)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_value;
  }
  hxc_string hxc_l_tmp_enum_payload_0_owner_n11 = hxc_l_path;
  if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n11) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_enum_payload_0_owned_load_result_n16 = hxc_l_tmp_enum_payload_0_owner_n11;
  hxc_string hxc_l_tmp_enum_payload_1_owner_n12 = hxc_l_value;
  if (hxc_string_retain(hxc_l_tmp_enum_payload_1_owner_n12) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_instance_call_argument_1_owner_n13 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidClosedValue, .hxc_payload.hxc_SchemaInvalidClosedValue.hxc_path = hxc_l_tmp_enum_payload_0_owned_load_result_n16, .hxc_payload.hxc_SchemaInvalidClosedValue.hxc_value = hxc_l_tmp_enum_payload_1_owner_n12 };
  hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_reader, hxc_l_node, hxc_l_tmp_instance_call_argument_1_owner_n13);
  hxc_string hxc_l_tmp_returned_string_owner_n14 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n14) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_returned_string_owned_load_result_n21 = hxc_l_tmp_returned_string_owner_n14;
  hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n13);
  if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_returned_string_owned_load_result_n21;
}

hxc_string hxc_caxecraft_content_RuntimeContentPack_readContentId(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, struct hxc_caxecraft_content_ContentJsonNode *hxc_l_node, hxc_string hxc_l_path)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_node };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_returned_string_owner_n4 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n6 = { 0 };
  if (hxc_l_reader == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeSchemaReader_string(hxc_l_reader, hxc_l_node, hxc_l_path, 128);
  hxc_string hxc_l_value = hxc_l_tmp_instance_call_result_n0;
  if (hxc_l_value.data == NULL)
  {
    hxc_l_tmp_returned_string_owner_n4 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n4) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_string hxc_l_tmp_returned_string_owned_load_result_n2 = hxc_l_tmp_returned_string_owner_n4;
    if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_returned_string_owned_load_result_n2;
  }
  bool hxc_l_tmp_call_result_n4 = hxc_caxecraft_content_RuntimeSchemaReader_validContentId(hxc_l_value);
  if (!!hxc_l_tmp_call_result_n4)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_value;
  }
  hxc_string hxc_l_tmp_enum_payload_0_owner_n5 = hxc_l_path;
  if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_instance_call_argument_1_owner_n6 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidContentId, .hxc_payload.hxc_SchemaInvalidContentId.hxc_path = hxc_l_tmp_enum_payload_0_owner_n5 };
  hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_reader, hxc_l_node, hxc_l_tmp_instance_call_argument_1_owner_n6);
  hxc_string hxc_l_tmp_returned_string_owner_n7 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n7) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_returned_string_owned_load_result_n8 = hxc_l_tmp_returned_string_owner_n7;
  hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n6);
  if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_returned_string_owned_load_result_n8;
}

struct hxc_array_ref *hxc_caxecraft_content_RuntimeContentPack_readDrops(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, struct hxc_caxecraft_content_ContentJsonNode *hxc_l_node, struct hxc_caxecraft_content_RuntimeAssetInventory *hxc_l_assets)
{
  const void *hxc_l_gc_roots[43] = { (const void *)hxc_l_node, (const void *)hxc_l_assets, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 43, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_string_concat_left_owner_n10 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_instance_call_argument_2_owner_n12 = { 0 };
  hxc_string hxc_l_id = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n15 = { 0 };
  hxc_string hxc_l_item = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n18 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n20 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n22 = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n24 = { 0 };
  hxc_string hxc_l_path = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n9 = { 0 };
  if (hxc_l_reader == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeSchemaReader_array(hxc_l_reader, hxc_l_node, (hxc_string){ (const uint8_t *)"drops", 5, true, NULL }, 0, 128);
  hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_instance_call_result_n0;
  struct hxc_array_ref *hxc_l_values = hxc_l_tmp_instance_call_result_n0;
  hxc_l_gc_roots[3] = (const void *)hxc_l_values;
  hxc_l_gc_roots[4] = (const void *)NULL;
  if (hxc_l_values == NULL)
  {
    hxc_l_gc_roots[5] = (const void *)NULL;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n2 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_array_a4eca502_descriptor, (void **)&hxc_l_tmp_array_create_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_init_in_place(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeDropDefinition *), _Alignof(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeDropDefinition *), NULL, NULL, NULL, NULL }, hxc_l_tmp_array_create_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[6] = (const void *)hxc_l_tmp_array_create_result_n2;
  struct hxc_array_ref *hxc_l_result = hxc_l_tmp_array_create_result_n2;
  int32_t hxc_l_g = 0;
  hxc_l_gc_roots[7] = (const void *)hxc_l_values;
  int32_t hxc_l_tmp_array_length_result_n4;
  if (hxc_array_ref_length(hxc_l_values, &hxc_l_tmp_array_length_result_n4) != HXC_STATUS_OK)
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
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n7;
    hxc_string hxc_l_tmp_std_string_int_result_n9 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_from_int32(hxc_l_index, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_right_owner_n9 = hxc_l_tmp_std_string_int_result_n9;
    hxc_string hxc_l_tmp_string_concat_result_n11 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"drops[", 6, true, NULL }, hxc_l_tmp_string_concat_right_owner_n9, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n11) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_left_owner_n10 = hxc_l_tmp_string_concat_result_n11;
    hxc_string hxc_l_tmp_string_concat_result_n13 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n10, (hxc_string){ (const uint8_t *)"]", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n13) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_path = hxc_l_tmp_string_concat_result_n13;
    struct hxc_array_ref *hxc_l_tmp_load_result_n14 = hxc_l_values;
    hxc_l_gc_roots[8] = (const void *)hxc_l_tmp_load_result_n14;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_array_get_result_n16;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n14, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n16) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[9] = (const void *)hxc_l_tmp_array_get_result_n16;
    hxc_string hxc_l_tmp_load_result_n17 = hxc_l_path;
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n18 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n18) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"id", 2, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"item", 4, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"quantity", 8, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"pickupRadiusMilli", 17, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"presentation", 12, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_2_owner_n12 = hxc_l_tmp_array_create_result_n18;
    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n20 = hxc_caxecraft_content_RuntimeSchemaReader_object(hxc_l_reader, hxc_l_tmp_array_get_result_n16, hxc_l_tmp_load_result_n17, hxc_l_tmp_instance_call_argument_2_owner_n12);
    hxc_l_gc_roots[10] = (const void *)hxc_l_tmp_instance_call_result_n20;
    struct hxc_array_ref *hxc_l_fields = hxc_l_tmp_instance_call_result_n20;
    hxc_l_gc_roots[11] = (const void *)hxc_l_fields;
    hxc_l_gc_roots[12] = (const void *)NULL;
    if (hxc_l_fields == NULL)
    {
      hxc_l_gc_roots[14] = (const void *)NULL;
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n12) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n10) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n9) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
    hxc_l_gc_roots[15] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n23 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"id", 2, true, NULL });
    hxc_l_gc_roots[16] = (const void *)hxc_l_tmp_instance_call_result_n23;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_idNode = hxc_l_tmp_instance_call_result_n23;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n24 = hxc_l_idNode;
    hxc_l_gc_roots[17] = (const void *)hxc_l_tmp_load_result_n24;
    hxc_string hxc_l_tmp_string_concat_result_n26 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".id", 3, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n26) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n15 = hxc_l_tmp_string_concat_result_n26;
    hxc_string hxc_l_tmp_call_result_n28 = hxc_caxecraft_content_RuntimeContentPack_readContentId(hxc_l_reader, hxc_l_tmp_load_result_n24, hxc_l_tmp_static_call_argument_2_owner_n15);
    hxc_l_id = hxc_l_tmp_call_result_n28;
    hxc_l_gc_roots[18] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n30 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"item", 4, true, NULL });
    hxc_l_gc_roots[19] = (const void *)hxc_l_tmp_instance_call_result_n30;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_itemNode = hxc_l_tmp_instance_call_result_n30;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n31 = hxc_l_itemNode;
    hxc_l_gc_roots[20] = (const void *)hxc_l_tmp_load_result_n31;
    hxc_string hxc_l_tmp_string_concat_result_n33 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".item", 5, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n33) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n18 = hxc_l_tmp_string_concat_result_n33;
    hxc_string hxc_l_tmp_call_result_n35 = hxc_caxecraft_content_RuntimeContentPack_readContentId(hxc_l_reader, hxc_l_tmp_load_result_n31, hxc_l_tmp_static_call_argument_2_owner_n18);
    hxc_l_item = hxc_l_tmp_call_result_n35;
    hxc_l_gc_roots[21] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n37 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"quantity", 8, true, NULL });
    hxc_l_gc_roots[22] = (const void *)hxc_l_tmp_instance_call_result_n37;
    hxc_string hxc_l_tmp_string_concat_result_n39 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".quantity", 9, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n39) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n20 = hxc_l_tmp_string_concat_result_n39;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_instance_call_result_n41 = hxc_caxecraft_content_RuntimeSchemaReader_integer(hxc_l_reader, hxc_l_tmp_instance_call_result_n37, hxc_l_tmp_instance_call_argument_1_owner_n20, 1, 64);
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_quantity = hxc_l_tmp_instance_call_result_n41;
    hxc_l_gc_roots[23] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n43 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"pickupRadiusMilli", 17, true, NULL });
    hxc_l_gc_roots[24] = (const void *)hxc_l_tmp_instance_call_result_n43;
    hxc_string hxc_l_tmp_string_concat_result_n45 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".pickupRadiusMilli", 18, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n45) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n22 = hxc_l_tmp_string_concat_result_n45;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_instance_call_result_n47 = hxc_caxecraft_content_RuntimeSchemaReader_integer(hxc_l_reader, hxc_l_tmp_instance_call_result_n43, hxc_l_tmp_instance_call_argument_1_owner_n22, 100, 32000);
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_radius = hxc_l_tmp_instance_call_result_n47;
    hxc_l_gc_roots[25] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n49 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"presentation", 12, true, NULL });
    hxc_l_gc_roots[26] = (const void *)hxc_l_tmp_instance_call_result_n49;
    hxc_string hxc_l_tmp_string_concat_result_n51 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".presentation", 13, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n51) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n24 = hxc_l_tmp_string_concat_result_n51;
    struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_call_result_n53 = hxc_caxecraft_content_RuntimeContentPack_readPresentation(hxc_l_reader, hxc_l_tmp_instance_call_result_n49, hxc_l_tmp_static_call_argument_2_owner_n24, hxc_l_assets);
    hxc_l_gc_roots[27] = (const void *)hxc_l_tmp_call_result_n53;
    struct hxc_caxecraft_content_RuntimePresentation *hxc_l_presentation = hxc_l_tmp_call_result_n53;
    struct hxc_optional_caxecraft_content_RuntimeSchemaDiagnostic hxc_l_tmp_class_field_load_result_n54 = (*hxc_l_reader).hxc_failure;
    bool hxc_l_tmp_short_circuit_result_n25 = hxc_l_tmp_class_field_load_result_n54.hxc_has_value;
    if (!hxc_l_tmp_class_field_load_result_n54.hxc_has_value)
    {
      hxc_l_tmp_short_circuit_result_n25 = hxc_l_id.data == NULL;
    }
    bool hxc_l_tmp_short_circuit_load_result_n56 = hxc_l_tmp_short_circuit_result_n25;
    bool hxc_l_tmp_short_circuit_result_n26 = hxc_l_tmp_short_circuit_load_result_n56;
    if (!hxc_l_tmp_short_circuit_load_result_n56)
    {
      hxc_l_tmp_short_circuit_result_n26 = hxc_l_item.data == NULL;
    }
    bool hxc_l_tmp_short_circuit_load_result_n58 = hxc_l_tmp_short_circuit_result_n26;
    bool hxc_l_tmp_short_circuit_result_n27 = hxc_l_tmp_short_circuit_load_result_n58;
    if (!hxc_l_tmp_short_circuit_load_result_n58)
    {
      hxc_l_tmp_short_circuit_result_n27 = !hxc_l_quantity.hxc_has_value;
    }
    bool hxc_l_tmp_short_circuit_load_result_n60 = hxc_l_tmp_short_circuit_result_n27;
    bool hxc_l_tmp_short_circuit_result_n28 = hxc_l_tmp_short_circuit_load_result_n60;
    if (!hxc_l_tmp_short_circuit_load_result_n60)
    {
      hxc_l_tmp_short_circuit_result_n28 = !hxc_l_radius.hxc_has_value;
    }
    bool hxc_l_tmp_short_circuit_load_result_n62 = hxc_l_tmp_short_circuit_result_n28;
    bool hxc_l_tmp_short_circuit_result_n29 = hxc_l_tmp_short_circuit_load_result_n62;
    if (!hxc_l_tmp_short_circuit_load_result_n62)
    {
      hxc_l_gc_roots[28] = (const void *)hxc_l_presentation;
      hxc_l_gc_roots[29] = (const void *)NULL;
      hxc_l_tmp_short_circuit_result_n29 = hxc_l_presentation == NULL;
    }
    if (hxc_l_tmp_short_circuit_result_n29)
    {
      hxc_l_gc_roots[30] = (const void *)NULL;
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n24) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n22) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n20) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_item) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n18) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n15) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n12) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n10) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n9) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n65 = hxc_l_result;
    hxc_l_gc_roots[31] = (const void *)hxc_l_tmp_load_result_n65;
    hxc_string hxc_l_tmp_load_result_n66 = hxc_l_id;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n67 = hxc_l_idNode;
    hxc_l_gc_roots[32] = (const void *)hxc_l_tmp_load_result_n67;
    if (hxc_l_tmp_load_result_n67 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n68 = (*hxc_l_tmp_load_result_n67).hxc_line;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n69 = hxc_l_idNode;
    hxc_l_gc_roots[33] = (const void *)hxc_l_tmp_load_result_n69;
    if (hxc_l_tmp_load_result_n69 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n70 = (*hxc_l_tmp_load_result_n69).hxc_column;
    hxc_string hxc_l_tmp_load_result_n71 = hxc_l_item;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n72 = hxc_l_itemNode;
    hxc_l_gc_roots[34] = (const void *)hxc_l_tmp_load_result_n72;
    if (hxc_l_tmp_load_result_n72 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n73 = (*hxc_l_tmp_load_result_n72).hxc_line;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n74 = hxc_l_itemNode;
    hxc_l_gc_roots[35] = (const void *)hxc_l_tmp_load_result_n74;
    if (hxc_l_tmp_load_result_n74 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n75 = (*hxc_l_tmp_load_result_n74).hxc_column;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_tmp_managed_class_result_n76 = NULL;
    if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_RuntimeContentPack_RuntimeReference_descriptor, (void **)&hxc_l_tmp_managed_class_result_n76) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[36] = (const void *)hxc_l_tmp_managed_class_result_n76;
    hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeReference(hxc_l_tmp_managed_class_result_n76, hxc_l_tmp_load_result_n71, hxc_l_tmp_class_field_load_result_n73, hxc_l_tmp_class_field_load_result_n75);
    hxc_l_gc_roots[37] = (const void *)hxc_l_tmp_managed_class_result_n76;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n77 = hxc_l_quantity;
    if (!hxc_l_tmp_load_result_n77.hxc_has_value)
    {
      abort();
    }
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n79 = hxc_l_radius;
    if (!hxc_l_tmp_load_result_n79.hxc_has_value)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_load_result_n81 = hxc_l_presentation;
    hxc_l_gc_roots[38] = (const void *)hxc_l_tmp_load_result_n81;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeDropDefinition *hxc_l_tmp_managed_class_result_n82 = NULL;
    if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_RuntimeContentPack_RuntimeDropDefinition_descriptor, (void **)&hxc_l_tmp_managed_class_result_n82) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[39] = (const void *)hxc_l_tmp_managed_class_result_n82;
    hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeDropDefinition(hxc_l_tmp_managed_class_result_n82, hxc_l_tmp_load_result_n66, hxc_l_tmp_class_field_load_result_n68, hxc_l_tmp_class_field_load_result_n70, hxc_l_tmp_managed_class_result_n76, hxc_l_tmp_load_result_n77.hxc_value, hxc_l_tmp_load_result_n79.hxc_value, hxc_l_tmp_load_result_n81);
    hxc_l_gc_roots[40] = (const void *)hxc_l_tmp_managed_class_result_n82;
    int32_t hxc_l_tmp_array_push_result_n83;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n65, &(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeDropDefinition *){ hxc_l_tmp_managed_class_result_n82 }, &hxc_l_tmp_array_push_result_n83) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n83;
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n24) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n22) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n20) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_item) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n18) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n15) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n12) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n10) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n9) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  hxc_l_gc_roots[13] = (const void *)hxc_l_result;
  bool hxc_l_tmp_call_result_n85 = hxc_caxecraft_content_RuntimeContentPack_validateDropOrder(hxc_l_reader, (hxc_string){ (const uint8_t *)"drops", 5, true, NULL }, hxc_l_result);
  if (!hxc_l_tmp_call_result_n85)
  {
    hxc_l_gc_roots[42] = (const void *)NULL;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  hxc_l_gc_roots[41] = (const void *)hxc_l_result;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_result;
}

struct hxc_array_ref *hxc_caxecraft_content_RuntimeContentPack_readEffects(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, struct hxc_caxecraft_content_ContentJsonNode *hxc_l_node)
{
  const void *hxc_l_gc_roots[27] = { (const void *)hxc_l_node, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 27, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_argument_2_owner_n11 = { 0 };
  hxc_string hxc_l_id = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n14 = { 0 };
  hxc_string hxc_l_profile = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n16 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_static_call_argument_3_owner_n17 = { 0 };
  hxc_string hxc_l_path = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n8 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n9 = { 0 };
  if (hxc_l_reader == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeSchemaReader_array(hxc_l_reader, hxc_l_node, (hxc_string){ (const uint8_t *)"effects", 7, true, NULL }, 0, 128);
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_instance_call_result_n0;
  struct hxc_array_ref *hxc_l_values = hxc_l_tmp_instance_call_result_n0;
  hxc_l_gc_roots[2] = (const void *)hxc_l_values;
  hxc_l_gc_roots[3] = (const void *)NULL;
  if (hxc_l_values == NULL)
  {
    hxc_l_gc_roots[4] = (const void *)NULL;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n2 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_array_08ae2cc0_descriptor, (void **)&hxc_l_tmp_array_create_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_init_in_place(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEffectDefinition *), _Alignof(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEffectDefinition *), NULL, NULL, NULL, NULL }, hxc_l_tmp_array_create_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_array_create_result_n2;
  struct hxc_array_ref *hxc_l_result = hxc_l_tmp_array_create_result_n2;
  int32_t hxc_l_g = 0;
  hxc_l_gc_roots[6] = (const void *)hxc_l_values;
  int32_t hxc_l_tmp_array_length_result_n4;
  if (hxc_array_ref_length(hxc_l_values, &hxc_l_tmp_array_length_result_n4) != HXC_STATUS_OK)
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
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n7;
    hxc_string hxc_l_tmp_std_string_int_result_n9 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_from_int32(hxc_l_index, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_right_owner_n8 = hxc_l_tmp_std_string_int_result_n9;
    hxc_string hxc_l_tmp_string_concat_result_n11 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"effects[", 8, true, NULL }, hxc_l_tmp_string_concat_right_owner_n8, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n11) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_left_owner_n9 = hxc_l_tmp_string_concat_result_n11;
    hxc_string hxc_l_tmp_string_concat_result_n13 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n9, (hxc_string){ (const uint8_t *)"]", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n13) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_path = hxc_l_tmp_string_concat_result_n13;
    struct hxc_array_ref *hxc_l_tmp_load_result_n14 = hxc_l_values;
    hxc_l_gc_roots[7] = (const void *)hxc_l_tmp_load_result_n14;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_array_get_result_n16;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n14, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n16) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[8] = (const void *)hxc_l_tmp_array_get_result_n16;
    hxc_string hxc_l_tmp_load_result_n17 = hxc_l_path;
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n18 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n18) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"id", 2, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"profile", 7, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_2_owner_n11 = hxc_l_tmp_array_create_result_n18;
    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n20 = hxc_caxecraft_content_RuntimeSchemaReader_object(hxc_l_reader, hxc_l_tmp_array_get_result_n16, hxc_l_tmp_load_result_n17, hxc_l_tmp_instance_call_argument_2_owner_n11);
    hxc_l_gc_roots[9] = (const void *)hxc_l_tmp_instance_call_result_n20;
    struct hxc_array_ref *hxc_l_fields = hxc_l_tmp_instance_call_result_n20;
    hxc_l_gc_roots[10] = (const void *)hxc_l_fields;
    hxc_l_gc_roots[11] = (const void *)NULL;
    if (hxc_l_fields == NULL)
    {
      hxc_l_gc_roots[13] = (const void *)NULL;
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n11) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n9) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n8) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
    hxc_l_gc_roots[14] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n23 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"id", 2, true, NULL });
    hxc_l_gc_roots[15] = (const void *)hxc_l_tmp_instance_call_result_n23;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_idNode = hxc_l_tmp_instance_call_result_n23;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n24 = hxc_l_idNode;
    hxc_l_gc_roots[16] = (const void *)hxc_l_tmp_load_result_n24;
    hxc_string hxc_l_tmp_string_concat_result_n26 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".id", 3, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n26) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n14 = hxc_l_tmp_string_concat_result_n26;
    hxc_string hxc_l_tmp_call_result_n28 = hxc_caxecraft_content_RuntimeContentPack_readContentId(hxc_l_reader, hxc_l_tmp_load_result_n24, hxc_l_tmp_static_call_argument_2_owner_n14);
    hxc_l_id = hxc_l_tmp_call_result_n28;
    hxc_l_gc_roots[17] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n30 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"profile", 7, true, NULL });
    hxc_l_gc_roots[18] = (const void *)hxc_l_tmp_instance_call_result_n30;
    hxc_string hxc_l_tmp_string_concat_result_n32 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".profile", 8, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n32) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n16 = hxc_l_tmp_string_concat_result_n32;
    hxc_string hxc_l_tmp_static_call_argument_2_borrow_result_n33 = hxc_l_tmp_static_call_argument_2_owner_n16;
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n34 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n34) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n34->value, &(hxc_string){ (const uint8_t *)"melee-feedback", 14, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n34->value, &(hxc_string){ (const uint8_t *)"pickup-feedback", 15, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_3_owner_n17 = hxc_l_tmp_array_create_result_n34;
    hxc_string hxc_l_tmp_call_result_n36 = hxc_caxecraft_content_RuntimeContentPack_readClosed(hxc_l_reader, hxc_l_tmp_instance_call_result_n30, hxc_l_tmp_static_call_argument_2_borrow_result_n33, hxc_l_tmp_static_call_argument_3_owner_n17);
    hxc_l_profile = hxc_l_tmp_call_result_n36;
    struct hxc_optional_caxecraft_content_RuntimeSchemaDiagnostic hxc_l_tmp_class_field_load_result_n37 = (*hxc_l_reader).hxc_failure;
    bool hxc_l_tmp_short_circuit_result_n18 = hxc_l_tmp_class_field_load_result_n37.hxc_has_value;
    if (!hxc_l_tmp_class_field_load_result_n37.hxc_has_value)
    {
      hxc_l_tmp_short_circuit_result_n18 = hxc_l_id.data == NULL;
    }
    bool hxc_l_tmp_short_circuit_load_result_n39 = hxc_l_tmp_short_circuit_result_n18;
    bool hxc_l_tmp_short_circuit_result_n19 = hxc_l_tmp_short_circuit_load_result_n39;
    if (!hxc_l_tmp_short_circuit_load_result_n39)
    {
      hxc_l_tmp_short_circuit_result_n19 = hxc_l_profile.data == NULL;
    }
    if (hxc_l_tmp_short_circuit_result_n19)
    {
      hxc_l_gc_roots[19] = (const void *)NULL;
      if (hxc_string_release(&hxc_l_profile) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n17) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n16) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n14) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n11) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n9) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n8) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n42 = hxc_l_result;
    hxc_l_gc_roots[20] = (const void *)hxc_l_tmp_load_result_n42;
    hxc_string hxc_l_tmp_load_result_n43 = hxc_l_id;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n44 = hxc_l_idNode;
    hxc_l_gc_roots[21] = (const void *)hxc_l_tmp_load_result_n44;
    if (hxc_l_tmp_load_result_n44 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n45 = (*hxc_l_tmp_load_result_n44).hxc_line;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n46 = hxc_l_idNode;
    hxc_l_gc_roots[22] = (const void *)hxc_l_tmp_load_result_n46;
    if (hxc_l_tmp_load_result_n46 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n47 = (*hxc_l_tmp_load_result_n46).hxc_column;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEffectDefinition *hxc_l_tmp_managed_class_result_n48 = NULL;
    if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_RuntimeContentPack_RuntimeEffectDefinition_descriptor, (void **)&hxc_l_tmp_managed_class_result_n48) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[23] = (const void *)hxc_l_tmp_managed_class_result_n48;
    hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeEffectDefinition(hxc_l_tmp_managed_class_result_n48, hxc_l_tmp_load_result_n43, hxc_l_tmp_class_field_load_result_n45, hxc_l_tmp_class_field_load_result_n47);
    hxc_l_gc_roots[24] = (const void *)hxc_l_tmp_managed_class_result_n48;
    int32_t hxc_l_tmp_array_push_result_n49;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n42, &(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEffectDefinition *){ hxc_l_tmp_managed_class_result_n48 }, &hxc_l_tmp_array_push_result_n49) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n49;
    if (hxc_string_release(&hxc_l_profile) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n17) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n16) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n14) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n11) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n8) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  hxc_l_gc_roots[12] = (const void *)hxc_l_result;
  bool hxc_l_tmp_call_result_n51 = hxc_caxecraft_content_RuntimeContentPack_validateEffectOrder(hxc_l_reader, (hxc_string){ (const uint8_t *)"effects", 7, true, NULL }, hxc_l_result);
  if (!hxc_l_tmp_call_result_n51)
  {
    hxc_l_gc_roots[26] = (const void *)NULL;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  hxc_l_gc_roots[25] = (const void *)hxc_l_result;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_result;
}

struct hxc_array_ref *hxc_caxecraft_content_RuntimeContentPack_readEnemies(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, struct hxc_caxecraft_content_ContentJsonNode *hxc_l_node, struct hxc_caxecraft_content_RuntimeAssetInventory *hxc_l_assets)
{
  const void *hxc_l_gc_roots[58] = { (const void *)hxc_l_node, (const void *)hxc_l_assets, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 58, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_string_concat_left_owner_n10 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_instance_call_argument_2_owner_n12 = { 0 };
  hxc_string hxc_l_id = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n15 = { 0 };
  hxc_string hxc_l_behavior = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n17 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_static_call_argument_3_owner_n18 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n20 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n22 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n24 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n26 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n28 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n30 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n32 = { 0 };
  hxc_string hxc_l_drop = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n35 = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n37 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n50 = { 0 };
  hxc_string hxc_l_path = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n9 = { 0 };
  if (hxc_l_reader == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeSchemaReader_array(hxc_l_reader, hxc_l_node, (hxc_string){ (const uint8_t *)"enemies", 7, true, NULL }, 0, 128);
  hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_instance_call_result_n0;
  struct hxc_array_ref *hxc_l_values = hxc_l_tmp_instance_call_result_n0;
  hxc_l_gc_roots[3] = (const void *)hxc_l_values;
  hxc_l_gc_roots[4] = (const void *)NULL;
  if (hxc_l_values == NULL)
  {
    hxc_l_gc_roots[5] = (const void *)NULL;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n2 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_array_3ee5a53e_descriptor, (void **)&hxc_l_tmp_array_create_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_init_in_place(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *), _Alignof(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *), NULL, NULL, NULL, NULL }, hxc_l_tmp_array_create_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[6] = (const void *)hxc_l_tmp_array_create_result_n2;
  struct hxc_array_ref *hxc_l_result = hxc_l_tmp_array_create_result_n2;
  int32_t hxc_l_g = 0;
  hxc_l_gc_roots[7] = (const void *)hxc_l_values;
  int32_t hxc_l_tmp_array_length_result_n4;
  if (hxc_array_ref_length(hxc_l_values, &hxc_l_tmp_array_length_result_n4) != HXC_STATUS_OK)
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
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n7;
    hxc_string hxc_l_tmp_std_string_int_result_n9 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_from_int32(hxc_l_index, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_right_owner_n9 = hxc_l_tmp_std_string_int_result_n9;
    hxc_string hxc_l_tmp_string_concat_result_n11 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"enemies[", 8, true, NULL }, hxc_l_tmp_string_concat_right_owner_n9, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n11) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_left_owner_n10 = hxc_l_tmp_string_concat_result_n11;
    hxc_string hxc_l_tmp_string_concat_result_n13 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n10, (hxc_string){ (const uint8_t *)"]", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n13) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_path = hxc_l_tmp_string_concat_result_n13;
    struct hxc_array_ref *hxc_l_tmp_load_result_n14 = hxc_l_values;
    hxc_l_gc_roots[8] = (const void *)hxc_l_tmp_load_result_n14;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_array_get_result_n16;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n14, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n16) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[9] = (const void *)hxc_l_tmp_array_get_result_n16;
    hxc_string hxc_l_tmp_load_result_n17 = hxc_l_path;
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n18 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n18) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"id", 2, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"behaviorProfile", 15, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"maxHealth", 9, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"noticeRadiusMilli", 17, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"strikeRadiusMilli", 17, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"attackRadiusMilli", 17, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"windupTicks", 11, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"recoveryTicks", 13, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"stepMilli", 9, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"drop", 4, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"presentation", 12, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_2_owner_n12 = hxc_l_tmp_array_create_result_n18;
    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n20 = hxc_caxecraft_content_RuntimeSchemaReader_object(hxc_l_reader, hxc_l_tmp_array_get_result_n16, hxc_l_tmp_load_result_n17, hxc_l_tmp_instance_call_argument_2_owner_n12);
    hxc_l_gc_roots[10] = (const void *)hxc_l_tmp_instance_call_result_n20;
    struct hxc_array_ref *hxc_l_fields = hxc_l_tmp_instance_call_result_n20;
    hxc_l_gc_roots[11] = (const void *)hxc_l_fields;
    hxc_l_gc_roots[12] = (const void *)NULL;
    if (hxc_l_fields == NULL)
    {
      hxc_l_gc_roots[14] = (const void *)NULL;
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n12) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n10) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n9) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
    hxc_l_gc_roots[15] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n23 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"id", 2, true, NULL });
    hxc_l_gc_roots[16] = (const void *)hxc_l_tmp_instance_call_result_n23;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_idNode = hxc_l_tmp_instance_call_result_n23;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n24 = hxc_l_idNode;
    hxc_l_gc_roots[17] = (const void *)hxc_l_tmp_load_result_n24;
    hxc_string hxc_l_tmp_string_concat_result_n26 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".id", 3, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n26) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n15 = hxc_l_tmp_string_concat_result_n26;
    hxc_string hxc_l_tmp_call_result_n28 = hxc_caxecraft_content_RuntimeContentPack_readContentId(hxc_l_reader, hxc_l_tmp_load_result_n24, hxc_l_tmp_static_call_argument_2_owner_n15);
    hxc_l_id = hxc_l_tmp_call_result_n28;
    hxc_l_gc_roots[18] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n30 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"behaviorProfile", 15, true, NULL });
    hxc_l_gc_roots[19] = (const void *)hxc_l_tmp_instance_call_result_n30;
    hxc_string hxc_l_tmp_string_concat_result_n32 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".behaviorProfile", 16, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n32) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n17 = hxc_l_tmp_string_concat_result_n32;
    hxc_string hxc_l_tmp_static_call_argument_2_borrow_result_n33 = hxc_l_tmp_static_call_argument_2_owner_n17;
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n34 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n34) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n34->value, &(hxc_string){ (const uint8_t *)"wander-chase-melee", 18, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_3_owner_n18 = hxc_l_tmp_array_create_result_n34;
    hxc_string hxc_l_tmp_call_result_n36 = hxc_caxecraft_content_RuntimeContentPack_readClosed(hxc_l_reader, hxc_l_tmp_instance_call_result_n30, hxc_l_tmp_static_call_argument_2_borrow_result_n33, hxc_l_tmp_static_call_argument_3_owner_n18);
    hxc_l_behavior = hxc_l_tmp_call_result_n36;
    hxc_l_gc_roots[20] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n38 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"maxHealth", 9, true, NULL });
    hxc_l_gc_roots[21] = (const void *)hxc_l_tmp_instance_call_result_n38;
    hxc_string hxc_l_tmp_string_concat_result_n40 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".maxHealth", 10, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n40) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n20 = hxc_l_tmp_string_concat_result_n40;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_instance_call_result_n42 = hxc_caxecraft_content_RuntimeSchemaReader_integer(hxc_l_reader, hxc_l_tmp_instance_call_result_n38, hxc_l_tmp_instance_call_argument_1_owner_n20, 1, 10000);
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_health = hxc_l_tmp_instance_call_result_n42;
    hxc_l_gc_roots[22] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n44 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"noticeRadiusMilli", 17, true, NULL });
    hxc_l_gc_roots[23] = (const void *)hxc_l_tmp_instance_call_result_n44;
    hxc_string hxc_l_tmp_string_concat_result_n46 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".noticeRadiusMilli", 18, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n46) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n22 = hxc_l_tmp_string_concat_result_n46;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_instance_call_result_n48 = hxc_caxecraft_content_RuntimeSchemaReader_integer(hxc_l_reader, hxc_l_tmp_instance_call_result_n44, hxc_l_tmp_instance_call_argument_1_owner_n22, 250, 64000);
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_notice = hxc_l_tmp_instance_call_result_n48;
    hxc_l_gc_roots[24] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n50 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"strikeRadiusMilli", 17, true, NULL });
    hxc_l_gc_roots[25] = (const void *)hxc_l_tmp_instance_call_result_n50;
    hxc_string hxc_l_tmp_string_concat_result_n52 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".strikeRadiusMilli", 18, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n52) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n24 = hxc_l_tmp_string_concat_result_n52;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_instance_call_result_n54 = hxc_caxecraft_content_RuntimeSchemaReader_integer(hxc_l_reader, hxc_l_tmp_instance_call_result_n50, hxc_l_tmp_instance_call_argument_1_owner_n24, 250, 64000);
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_strike = hxc_l_tmp_instance_call_result_n54;
    hxc_l_gc_roots[26] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n56 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"attackRadiusMilli", 17, true, NULL });
    hxc_l_gc_roots[27] = (const void *)hxc_l_tmp_instance_call_result_n56;
    hxc_string hxc_l_tmp_string_concat_result_n58 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".attackRadiusMilli", 18, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n58) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n26 = hxc_l_tmp_string_concat_result_n58;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_instance_call_result_n60 = hxc_caxecraft_content_RuntimeSchemaReader_integer(hxc_l_reader, hxc_l_tmp_instance_call_result_n56, hxc_l_tmp_instance_call_argument_1_owner_n26, 250, 64000);
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_attack = hxc_l_tmp_instance_call_result_n60;
    hxc_l_gc_roots[28] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n62 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"windupTicks", 11, true, NULL });
    hxc_l_gc_roots[29] = (const void *)hxc_l_tmp_instance_call_result_n62;
    hxc_string hxc_l_tmp_string_concat_result_n64 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".windupTicks", 12, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n64) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n28 = hxc_l_tmp_string_concat_result_n64;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_instance_call_result_n66 = hxc_caxecraft_content_RuntimeSchemaReader_integer(hxc_l_reader, hxc_l_tmp_instance_call_result_n62, hxc_l_tmp_instance_call_argument_1_owner_n28, 1, 1200);
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_windup = hxc_l_tmp_instance_call_result_n66;
    hxc_l_gc_roots[30] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n68 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"recoveryTicks", 13, true, NULL });
    hxc_l_gc_roots[31] = (const void *)hxc_l_tmp_instance_call_result_n68;
    hxc_string hxc_l_tmp_string_concat_result_n70 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".recoveryTicks", 14, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n70) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n30 = hxc_l_tmp_string_concat_result_n70;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_instance_call_result_n72 = hxc_caxecraft_content_RuntimeSchemaReader_integer(hxc_l_reader, hxc_l_tmp_instance_call_result_n68, hxc_l_tmp_instance_call_argument_1_owner_n30, 1, 1200);
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_recovery = hxc_l_tmp_instance_call_result_n72;
    hxc_l_gc_roots[32] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n74 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"stepMilli", 9, true, NULL });
    hxc_l_gc_roots[33] = (const void *)hxc_l_tmp_instance_call_result_n74;
    hxc_string hxc_l_tmp_string_concat_result_n76 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".stepMilli", 10, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n76) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n32 = hxc_l_tmp_string_concat_result_n76;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_instance_call_result_n78 = hxc_caxecraft_content_RuntimeSchemaReader_integer(hxc_l_reader, hxc_l_tmp_instance_call_result_n74, hxc_l_tmp_instance_call_argument_1_owner_n32, 1, 10000);
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_step = hxc_l_tmp_instance_call_result_n78;
    hxc_l_gc_roots[34] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n80 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"drop", 4, true, NULL });
    hxc_l_gc_roots[35] = (const void *)hxc_l_tmp_instance_call_result_n80;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_dropNode = hxc_l_tmp_instance_call_result_n80;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n81 = hxc_l_dropNode;
    hxc_l_gc_roots[36] = (const void *)hxc_l_tmp_load_result_n81;
    hxc_string hxc_l_tmp_string_concat_result_n83 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".drop", 5, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n83) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n35 = hxc_l_tmp_string_concat_result_n83;
    hxc_string hxc_l_tmp_call_result_n85 = hxc_caxecraft_content_RuntimeContentPack_readContentId(hxc_l_reader, hxc_l_tmp_load_result_n81, hxc_l_tmp_static_call_argument_2_owner_n35);
    hxc_l_drop = hxc_l_tmp_call_result_n85;
    hxc_l_gc_roots[37] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n87 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"presentation", 12, true, NULL });
    hxc_l_gc_roots[38] = (const void *)hxc_l_tmp_instance_call_result_n87;
    hxc_string hxc_l_tmp_string_concat_result_n89 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".presentation", 13, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n89) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n37 = hxc_l_tmp_string_concat_result_n89;
    struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_call_result_n91 = hxc_caxecraft_content_RuntimeContentPack_readActorPresentation(hxc_l_reader, hxc_l_tmp_instance_call_result_n87, hxc_l_tmp_static_call_argument_2_owner_n37, hxc_l_assets);
    hxc_l_gc_roots[39] = (const void *)hxc_l_tmp_call_result_n91;
    struct hxc_caxecraft_content_RuntimePresentation *hxc_l_presentation = hxc_l_tmp_call_result_n91;
    struct hxc_optional_caxecraft_content_RuntimeSchemaDiagnostic hxc_l_tmp_class_field_load_result_n92 = (*hxc_l_reader).hxc_failure;
    bool hxc_l_tmp_short_circuit_result_n38 = hxc_l_tmp_class_field_load_result_n92.hxc_has_value;
    if (!hxc_l_tmp_class_field_load_result_n92.hxc_has_value)
    {
      hxc_l_tmp_short_circuit_result_n38 = hxc_l_id.data == NULL;
    }
    bool hxc_l_tmp_short_circuit_load_result_n94 = hxc_l_tmp_short_circuit_result_n38;
    bool hxc_l_tmp_short_circuit_result_n39 = hxc_l_tmp_short_circuit_load_result_n94;
    if (!hxc_l_tmp_short_circuit_load_result_n94)
    {
      hxc_l_tmp_short_circuit_result_n39 = hxc_l_behavior.data == NULL;
    }
    bool hxc_l_tmp_short_circuit_load_result_n96 = hxc_l_tmp_short_circuit_result_n39;
    bool hxc_l_tmp_short_circuit_result_n40 = hxc_l_tmp_short_circuit_load_result_n96;
    if (!hxc_l_tmp_short_circuit_load_result_n96)
    {
      hxc_l_tmp_short_circuit_result_n40 = !hxc_l_health.hxc_has_value;
    }
    bool hxc_l_tmp_short_circuit_load_result_n98 = hxc_l_tmp_short_circuit_result_n40;
    bool hxc_l_tmp_short_circuit_result_n41 = hxc_l_tmp_short_circuit_load_result_n98;
    if (!hxc_l_tmp_short_circuit_load_result_n98)
    {
      hxc_l_tmp_short_circuit_result_n41 = !hxc_l_notice.hxc_has_value;
    }
    bool hxc_l_tmp_short_circuit_load_result_n100 = hxc_l_tmp_short_circuit_result_n41;
    bool hxc_l_tmp_short_circuit_result_n42 = hxc_l_tmp_short_circuit_load_result_n100;
    if (!hxc_l_tmp_short_circuit_load_result_n100)
    {
      hxc_l_tmp_short_circuit_result_n42 = !hxc_l_strike.hxc_has_value;
    }
    bool hxc_l_tmp_short_circuit_load_result_n102 = hxc_l_tmp_short_circuit_result_n42;
    bool hxc_l_tmp_short_circuit_result_n43 = hxc_l_tmp_short_circuit_load_result_n102;
    if (!hxc_l_tmp_short_circuit_load_result_n102)
    {
      hxc_l_tmp_short_circuit_result_n43 = !hxc_l_attack.hxc_has_value;
    }
    bool hxc_l_tmp_short_circuit_load_result_n104 = hxc_l_tmp_short_circuit_result_n43;
    bool hxc_l_tmp_short_circuit_result_n44 = hxc_l_tmp_short_circuit_load_result_n104;
    if (!hxc_l_tmp_short_circuit_load_result_n104)
    {
      hxc_l_tmp_short_circuit_result_n44 = !hxc_l_windup.hxc_has_value;
    }
    bool hxc_l_tmp_short_circuit_load_result_n106 = hxc_l_tmp_short_circuit_result_n44;
    bool hxc_l_tmp_short_circuit_result_n45 = hxc_l_tmp_short_circuit_load_result_n106;
    if (!hxc_l_tmp_short_circuit_load_result_n106)
    {
      hxc_l_tmp_short_circuit_result_n45 = !hxc_l_recovery.hxc_has_value;
    }
    bool hxc_l_tmp_short_circuit_load_result_n108 = hxc_l_tmp_short_circuit_result_n45;
    bool hxc_l_tmp_short_circuit_result_n46 = hxc_l_tmp_short_circuit_load_result_n108;
    if (!hxc_l_tmp_short_circuit_load_result_n108)
    {
      hxc_l_tmp_short_circuit_result_n46 = !hxc_l_step.hxc_has_value;
    }
    bool hxc_l_tmp_short_circuit_load_result_n110 = hxc_l_tmp_short_circuit_result_n46;
    bool hxc_l_tmp_short_circuit_result_n47 = hxc_l_tmp_short_circuit_load_result_n110;
    if (!hxc_l_tmp_short_circuit_load_result_n110)
    {
      hxc_l_tmp_short_circuit_result_n47 = hxc_l_drop.data == NULL;
    }
    bool hxc_l_tmp_short_circuit_load_result_n112 = hxc_l_tmp_short_circuit_result_n47;
    bool hxc_l_tmp_short_circuit_result_n48 = hxc_l_tmp_short_circuit_load_result_n112;
    if (!hxc_l_tmp_short_circuit_load_result_n112)
    {
      hxc_l_gc_roots[40] = (const void *)hxc_l_presentation;
      hxc_l_gc_roots[41] = (const void *)NULL;
      hxc_l_tmp_short_circuit_result_n48 = hxc_l_presentation == NULL;
    }
    if (hxc_l_tmp_short_circuit_result_n48)
    {
      hxc_l_gc_roots[42] = (const void *)NULL;
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n37) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_drop) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n35) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n32) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n30) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n28) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n26) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n24) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n22) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n20) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_behavior) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n18) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n17) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n15) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n12) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n10) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n9) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n115 = hxc_l_attack;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n116 = hxc_l_strike;
    if (!hxc_l_tmp_load_result_n115.hxc_has_value)
    {
      abort();
    }
    if (!hxc_l_tmp_load_result_n116.hxc_has_value)
    {
      abort();
    }
    bool hxc_l_tmp_short_circuit_result_n49 = hxc_l_tmp_load_result_n115.hxc_value <= hxc_l_tmp_load_result_n116.hxc_value;
    if (hxc_l_tmp_load_result_n115.hxc_value <= hxc_l_tmp_load_result_n116.hxc_value)
    {
      struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n119 = hxc_l_strike;
      struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n120 = hxc_l_notice;
      if (!hxc_l_tmp_load_result_n119.hxc_has_value)
      {
        abort();
      }
      if (!hxc_l_tmp_load_result_n120.hxc_has_value)
      {
        abort();
      }
      hxc_l_tmp_short_circuit_result_n49 = hxc_l_tmp_load_result_n119.hxc_value <= hxc_l_tmp_load_result_n120.hxc_value;
    }
    if (!hxc_l_tmp_short_circuit_result_n49)
    {
      struct hxc_array_ref *hxc_l_tmp_load_result_n124 = hxc_l_values;
      hxc_l_gc_roots[43] = (const void *)hxc_l_tmp_load_result_n124;
      struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_array_get_result_n126;
      if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n124, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n126) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_gc_roots[44] = (const void *)hxc_l_tmp_array_get_result_n126;
      hxc_string hxc_l_tmp_string_concat_result_n128 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".attackRadiusMilli", 18, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n128) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_instance_call_argument_1_owner_n50 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidInvariant, .hxc_payload.hxc_SchemaInvalidInvariant.hxc_path = hxc_l_tmp_string_concat_result_n128 };
      hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_reader, hxc_l_tmp_array_get_result_n126, hxc_l_tmp_instance_call_argument_1_owner_n50);
      hxc_l_gc_roots[45] = (const void *)NULL;
      hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n50);
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n37) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_drop) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n35) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n32) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n30) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n28) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n26) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n24) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n22) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n20) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_behavior) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n18) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n17) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n15) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n12) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n10) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n9) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n131 = hxc_l_result;
    hxc_l_gc_roots[46] = (const void *)hxc_l_tmp_load_result_n131;
    hxc_string hxc_l_tmp_load_result_n132 = hxc_l_id;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n133 = hxc_l_idNode;
    hxc_l_gc_roots[47] = (const void *)hxc_l_tmp_load_result_n133;
    if (hxc_l_tmp_load_result_n133 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n134 = (*hxc_l_tmp_load_result_n133).hxc_line;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n135 = hxc_l_idNode;
    hxc_l_gc_roots[48] = (const void *)hxc_l_tmp_load_result_n135;
    if (hxc_l_tmp_load_result_n135 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n136 = (*hxc_l_tmp_load_result_n135).hxc_column;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n137 = hxc_l_health;
    if (!hxc_l_tmp_load_result_n137.hxc_has_value)
    {
      abort();
    }
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n139 = hxc_l_notice;
    if (!hxc_l_tmp_load_result_n139.hxc_has_value)
    {
      abort();
    }
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n141 = hxc_l_strike;
    if (!hxc_l_tmp_load_result_n141.hxc_has_value)
    {
      abort();
    }
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n143 = hxc_l_attack;
    if (!hxc_l_tmp_load_result_n143.hxc_has_value)
    {
      abort();
    }
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n145 = hxc_l_windup;
    if (!hxc_l_tmp_load_result_n145.hxc_has_value)
    {
      abort();
    }
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n147 = hxc_l_recovery;
    if (!hxc_l_tmp_load_result_n147.hxc_has_value)
    {
      abort();
    }
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n149 = hxc_l_step;
    if (!hxc_l_tmp_load_result_n149.hxc_has_value)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n151 = hxc_l_drop;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n152 = hxc_l_dropNode;
    hxc_l_gc_roots[49] = (const void *)hxc_l_tmp_load_result_n152;
    if (hxc_l_tmp_load_result_n152 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n153 = (*hxc_l_tmp_load_result_n152).hxc_line;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n154 = hxc_l_dropNode;
    hxc_l_gc_roots[50] = (const void *)hxc_l_tmp_load_result_n154;
    if (hxc_l_tmp_load_result_n154 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n155 = (*hxc_l_tmp_load_result_n154).hxc_column;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_tmp_managed_class_result_n156 = NULL;
    if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_RuntimeContentPack_RuntimeReference_descriptor, (void **)&hxc_l_tmp_managed_class_result_n156) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[51] = (const void *)hxc_l_tmp_managed_class_result_n156;
    hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeReference(hxc_l_tmp_managed_class_result_n156, hxc_l_tmp_load_result_n151, hxc_l_tmp_class_field_load_result_n153, hxc_l_tmp_class_field_load_result_n155);
    hxc_l_gc_roots[52] = (const void *)hxc_l_tmp_managed_class_result_n156;
    struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_load_result_n157 = hxc_l_presentation;
    hxc_l_gc_roots[53] = (const void *)hxc_l_tmp_load_result_n157;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_tmp_managed_class_result_n158 = NULL;
    if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition_descriptor, (void **)&hxc_l_tmp_managed_class_result_n158) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[54] = (const void *)hxc_l_tmp_managed_class_result_n158;
    hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition(hxc_l_tmp_managed_class_result_n158, hxc_l_tmp_load_result_n132, hxc_l_tmp_class_field_load_result_n134, hxc_l_tmp_class_field_load_result_n136, hxc_l_tmp_load_result_n137.hxc_value, hxc_l_tmp_load_result_n139.hxc_value, hxc_l_tmp_load_result_n141.hxc_value, hxc_l_tmp_load_result_n143.hxc_value, hxc_l_tmp_load_result_n145.hxc_value, hxc_l_tmp_load_result_n147.hxc_value, hxc_l_tmp_load_result_n149.hxc_value, hxc_l_tmp_managed_class_result_n156, hxc_l_tmp_load_result_n157);
    hxc_l_gc_roots[55] = (const void *)hxc_l_tmp_managed_class_result_n158;
    int32_t hxc_l_tmp_array_push_result_n159;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n131, &(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *){ hxc_l_tmp_managed_class_result_n158 }, &hxc_l_tmp_array_push_result_n159) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n159;
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n37) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_drop) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n35) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n32) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n30) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n28) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n26) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n24) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n22) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n20) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_behavior) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n18) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n17) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n15) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n12) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n10) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n9) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  hxc_l_gc_roots[13] = (const void *)hxc_l_result;
  bool hxc_l_tmp_call_result_n161 = hxc_caxecraft_content_RuntimeContentPack_validateEnemyOrder(hxc_l_reader, (hxc_string){ (const uint8_t *)"enemies", 7, true, NULL }, hxc_l_result);
  if (!hxc_l_tmp_call_result_n161)
  {
    hxc_l_gc_roots[57] = (const void *)NULL;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  hxc_l_gc_roots[56] = (const void *)hxc_l_result;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_result;
}

struct hxc_array_ref *hxc_caxecraft_content_RuntimeContentPack_readFluids(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, struct hxc_caxecraft_content_ContentJsonNode *hxc_l_node, struct hxc_caxecraft_content_RuntimeAssetInventory *hxc_l_assets)
{
  const void *hxc_l_gc_roots[40] = { (const void *)hxc_l_node, (const void *)hxc_l_assets, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 40, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_string_concat_left_owner_n10 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_instance_call_argument_2_owner_n12 = { 0 };
  hxc_string hxc_l_id = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n15 = { 0 };
  hxc_string hxc_l_simulation = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n17 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_static_call_argument_3_owner_n18 = { 0 };
  hxc_string hxc_l_render = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n20 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_static_call_argument_3_owner_n21 = { 0 };
  hxc_string hxc_l_camera = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n23 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_static_call_argument_3_owner_n24 = { 0 };
  hxc_string hxc_l_audio = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n26 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_static_call_argument_3_owner_n27 = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n29 = { 0 };
  hxc_string hxc_l_path = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n9 = { 0 };
  if (hxc_l_reader == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeSchemaReader_array(hxc_l_reader, hxc_l_node, (hxc_string){ (const uint8_t *)"fluids", 6, true, NULL }, 1, 128);
  hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_instance_call_result_n0;
  struct hxc_array_ref *hxc_l_values = hxc_l_tmp_instance_call_result_n0;
  hxc_l_gc_roots[3] = (const void *)hxc_l_values;
  hxc_l_gc_roots[4] = (const void *)NULL;
  if (hxc_l_values == NULL)
  {
    hxc_l_gc_roots[5] = (const void *)NULL;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n2 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_array_a482c2d4_descriptor, (void **)&hxc_l_tmp_array_create_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_init_in_place(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeFluidDefinition *), _Alignof(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeFluidDefinition *), NULL, NULL, NULL, NULL }, hxc_l_tmp_array_create_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[6] = (const void *)hxc_l_tmp_array_create_result_n2;
  struct hxc_array_ref *hxc_l_result = hxc_l_tmp_array_create_result_n2;
  int32_t hxc_l_g = 0;
  hxc_l_gc_roots[7] = (const void *)hxc_l_values;
  int32_t hxc_l_tmp_array_length_result_n4;
  if (hxc_array_ref_length(hxc_l_values, &hxc_l_tmp_array_length_result_n4) != HXC_STATUS_OK)
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
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n7;
    hxc_string hxc_l_tmp_std_string_int_result_n9 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_from_int32(hxc_l_index, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_right_owner_n9 = hxc_l_tmp_std_string_int_result_n9;
    hxc_string hxc_l_tmp_string_concat_result_n11 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"fluids[", 7, true, NULL }, hxc_l_tmp_string_concat_right_owner_n9, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n11) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_left_owner_n10 = hxc_l_tmp_string_concat_result_n11;
    hxc_string hxc_l_tmp_string_concat_result_n13 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n10, (hxc_string){ (const uint8_t *)"]", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n13) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_path = hxc_l_tmp_string_concat_result_n13;
    struct hxc_array_ref *hxc_l_tmp_load_result_n14 = hxc_l_values;
    hxc_l_gc_roots[8] = (const void *)hxc_l_tmp_load_result_n14;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_array_get_result_n16;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n14, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n16) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[9] = (const void *)hxc_l_tmp_array_get_result_n16;
    hxc_string hxc_l_tmp_load_result_n17 = hxc_l_path;
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n18 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n18) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"id", 2, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"simulationProfile", 17, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"renderProfile", 13, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"cameraProfile", 13, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"audioProfile", 12, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"presentation", 12, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_2_owner_n12 = hxc_l_tmp_array_create_result_n18;
    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n20 = hxc_caxecraft_content_RuntimeSchemaReader_object(hxc_l_reader, hxc_l_tmp_array_get_result_n16, hxc_l_tmp_load_result_n17, hxc_l_tmp_instance_call_argument_2_owner_n12);
    hxc_l_gc_roots[10] = (const void *)hxc_l_tmp_instance_call_result_n20;
    struct hxc_array_ref *hxc_l_fields = hxc_l_tmp_instance_call_result_n20;
    hxc_l_gc_roots[11] = (const void *)hxc_l_fields;
    hxc_l_gc_roots[12] = (const void *)NULL;
    if (hxc_l_fields == NULL)
    {
      hxc_l_gc_roots[14] = (const void *)NULL;
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n12) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n10) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n9) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
    hxc_l_gc_roots[15] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n23 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"id", 2, true, NULL });
    hxc_l_gc_roots[16] = (const void *)hxc_l_tmp_instance_call_result_n23;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_idNode = hxc_l_tmp_instance_call_result_n23;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n24 = hxc_l_idNode;
    hxc_l_gc_roots[17] = (const void *)hxc_l_tmp_load_result_n24;
    hxc_string hxc_l_tmp_string_concat_result_n26 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".id", 3, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n26) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n15 = hxc_l_tmp_string_concat_result_n26;
    hxc_string hxc_l_tmp_call_result_n28 = hxc_caxecraft_content_RuntimeContentPack_readContentId(hxc_l_reader, hxc_l_tmp_load_result_n24, hxc_l_tmp_static_call_argument_2_owner_n15);
    hxc_l_id = hxc_l_tmp_call_result_n28;
    hxc_l_gc_roots[18] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n30 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"simulationProfile", 17, true, NULL });
    hxc_l_gc_roots[19] = (const void *)hxc_l_tmp_instance_call_result_n30;
    hxc_string hxc_l_tmp_string_concat_result_n32 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".simulationProfile", 18, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n32) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n17 = hxc_l_tmp_string_concat_result_n32;
    hxc_string hxc_l_tmp_static_call_argument_2_borrow_result_n33 = hxc_l_tmp_static_call_argument_2_owner_n17;
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n34 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n34) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n34->value, &(hxc_string){ (const uint8_t *)"bounded-water", 13, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_3_owner_n18 = hxc_l_tmp_array_create_result_n34;
    hxc_string hxc_l_tmp_call_result_n36 = hxc_caxecraft_content_RuntimeContentPack_readClosed(hxc_l_reader, hxc_l_tmp_instance_call_result_n30, hxc_l_tmp_static_call_argument_2_borrow_result_n33, hxc_l_tmp_static_call_argument_3_owner_n18);
    hxc_l_simulation = hxc_l_tmp_call_result_n36;
    hxc_l_gc_roots[20] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n38 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"renderProfile", 13, true, NULL });
    hxc_l_gc_roots[21] = (const void *)hxc_l_tmp_instance_call_result_n38;
    hxc_string hxc_l_tmp_string_concat_result_n40 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".renderProfile", 14, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n40) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n20 = hxc_l_tmp_string_concat_result_n40;
    hxc_string hxc_l_tmp_static_call_argument_2_borrow_result_n41 = hxc_l_tmp_static_call_argument_2_owner_n20;
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n42 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n42) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n42->value, &(hxc_string){ (const uint8_t *)"translucent-voxel", 17, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_3_owner_n21 = hxc_l_tmp_array_create_result_n42;
    hxc_string hxc_l_tmp_call_result_n44 = hxc_caxecraft_content_RuntimeContentPack_readClosed(hxc_l_reader, hxc_l_tmp_instance_call_result_n38, hxc_l_tmp_static_call_argument_2_borrow_result_n41, hxc_l_tmp_static_call_argument_3_owner_n21);
    hxc_l_render = hxc_l_tmp_call_result_n44;
    hxc_l_gc_roots[22] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n46 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"cameraProfile", 13, true, NULL });
    hxc_l_gc_roots[23] = (const void *)hxc_l_tmp_instance_call_result_n46;
    hxc_string hxc_l_tmp_string_concat_result_n48 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".cameraProfile", 14, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n48) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n23 = hxc_l_tmp_string_concat_result_n48;
    hxc_string hxc_l_tmp_static_call_argument_2_borrow_result_n49 = hxc_l_tmp_static_call_argument_2_owner_n23;
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n50 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n50) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n50->value, &(hxc_string){ (const uint8_t *)"clear-submersion", 16, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_3_owner_n24 = hxc_l_tmp_array_create_result_n50;
    hxc_string hxc_l_tmp_call_result_n52 = hxc_caxecraft_content_RuntimeContentPack_readClosed(hxc_l_reader, hxc_l_tmp_instance_call_result_n46, hxc_l_tmp_static_call_argument_2_borrow_result_n49, hxc_l_tmp_static_call_argument_3_owner_n24);
    hxc_l_camera = hxc_l_tmp_call_result_n52;
    hxc_l_gc_roots[24] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n54 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"audioProfile", 12, true, NULL });
    hxc_l_gc_roots[25] = (const void *)hxc_l_tmp_instance_call_result_n54;
    hxc_string hxc_l_tmp_string_concat_result_n56 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".audioProfile", 13, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n56) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n26 = hxc_l_tmp_string_concat_result_n56;
    hxc_string hxc_l_tmp_static_call_argument_2_borrow_result_n57 = hxc_l_tmp_static_call_argument_2_owner_n26;
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n58 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n58) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n58->value, &(hxc_string){ (const uint8_t *)"fresh-water", 11, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_3_owner_n27 = hxc_l_tmp_array_create_result_n58;
    hxc_string hxc_l_tmp_call_result_n60 = hxc_caxecraft_content_RuntimeContentPack_readClosed(hxc_l_reader, hxc_l_tmp_instance_call_result_n54, hxc_l_tmp_static_call_argument_2_borrow_result_n57, hxc_l_tmp_static_call_argument_3_owner_n27);
    hxc_l_audio = hxc_l_tmp_call_result_n60;
    hxc_l_gc_roots[26] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n62 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"presentation", 12, true, NULL });
    hxc_l_gc_roots[27] = (const void *)hxc_l_tmp_instance_call_result_n62;
    hxc_string hxc_l_tmp_string_concat_result_n64 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".presentation", 13, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n64) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n29 = hxc_l_tmp_string_concat_result_n64;
    struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_call_result_n66 = hxc_caxecraft_content_RuntimeContentPack_readPresentation(hxc_l_reader, hxc_l_tmp_instance_call_result_n62, hxc_l_tmp_static_call_argument_2_owner_n29, hxc_l_assets);
    hxc_l_gc_roots[28] = (const void *)hxc_l_tmp_call_result_n66;
    struct hxc_caxecraft_content_RuntimePresentation *hxc_l_presentation = hxc_l_tmp_call_result_n66;
    struct hxc_optional_caxecraft_content_RuntimeSchemaDiagnostic hxc_l_tmp_class_field_load_result_n67 = (*hxc_l_reader).hxc_failure;
    bool hxc_l_tmp_short_circuit_result_n30 = hxc_l_tmp_class_field_load_result_n67.hxc_has_value;
    if (!hxc_l_tmp_class_field_load_result_n67.hxc_has_value)
    {
      hxc_l_tmp_short_circuit_result_n30 = hxc_l_id.data == NULL;
    }
    bool hxc_l_tmp_short_circuit_load_result_n69 = hxc_l_tmp_short_circuit_result_n30;
    bool hxc_l_tmp_short_circuit_result_n31 = hxc_l_tmp_short_circuit_load_result_n69;
    if (!hxc_l_tmp_short_circuit_load_result_n69)
    {
      hxc_l_tmp_short_circuit_result_n31 = hxc_l_simulation.data == NULL;
    }
    bool hxc_l_tmp_short_circuit_load_result_n71 = hxc_l_tmp_short_circuit_result_n31;
    bool hxc_l_tmp_short_circuit_result_n32 = hxc_l_tmp_short_circuit_load_result_n71;
    if (!hxc_l_tmp_short_circuit_load_result_n71)
    {
      hxc_l_tmp_short_circuit_result_n32 = hxc_l_render.data == NULL;
    }
    bool hxc_l_tmp_short_circuit_load_result_n73 = hxc_l_tmp_short_circuit_result_n32;
    bool hxc_l_tmp_short_circuit_result_n33 = hxc_l_tmp_short_circuit_load_result_n73;
    if (!hxc_l_tmp_short_circuit_load_result_n73)
    {
      hxc_l_tmp_short_circuit_result_n33 = hxc_l_camera.data == NULL;
    }
    bool hxc_l_tmp_short_circuit_load_result_n75 = hxc_l_tmp_short_circuit_result_n33;
    bool hxc_l_tmp_short_circuit_result_n34 = hxc_l_tmp_short_circuit_load_result_n75;
    if (!hxc_l_tmp_short_circuit_load_result_n75)
    {
      hxc_l_tmp_short_circuit_result_n34 = hxc_l_audio.data == NULL;
    }
    bool hxc_l_tmp_short_circuit_load_result_n77 = hxc_l_tmp_short_circuit_result_n34;
    bool hxc_l_tmp_short_circuit_result_n35 = hxc_l_tmp_short_circuit_load_result_n77;
    if (!hxc_l_tmp_short_circuit_load_result_n77)
    {
      hxc_l_gc_roots[29] = (const void *)hxc_l_presentation;
      hxc_l_gc_roots[30] = (const void *)NULL;
      hxc_l_tmp_short_circuit_result_n35 = hxc_l_presentation == NULL;
    }
    if (hxc_l_tmp_short_circuit_result_n35)
    {
      hxc_l_gc_roots[31] = (const void *)NULL;
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n29) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_audio) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n27) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n26) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_camera) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n24) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n23) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_render) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n21) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n20) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_simulation) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n18) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n17) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n15) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n12) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n10) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n9) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n80 = hxc_l_result;
    hxc_l_gc_roots[32] = (const void *)hxc_l_tmp_load_result_n80;
    hxc_string hxc_l_tmp_load_result_n81 = hxc_l_id;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n82 = hxc_l_idNode;
    hxc_l_gc_roots[33] = (const void *)hxc_l_tmp_load_result_n82;
    if (hxc_l_tmp_load_result_n82 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n83 = (*hxc_l_tmp_load_result_n82).hxc_line;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n84 = hxc_l_idNode;
    hxc_l_gc_roots[34] = (const void *)hxc_l_tmp_load_result_n84;
    if (hxc_l_tmp_load_result_n84 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n85 = (*hxc_l_tmp_load_result_n84).hxc_column;
    struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_load_result_n86 = hxc_l_presentation;
    hxc_l_gc_roots[35] = (const void *)hxc_l_tmp_load_result_n86;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeFluidDefinition *hxc_l_tmp_managed_class_result_n87 = NULL;
    if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_RuntimeContentPack_RuntimeFluidDefinition_descriptor, (void **)&hxc_l_tmp_managed_class_result_n87) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[36] = (const void *)hxc_l_tmp_managed_class_result_n87;
    hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeFluidDefinition(hxc_l_tmp_managed_class_result_n87, hxc_l_tmp_load_result_n81, hxc_l_tmp_class_field_load_result_n83, hxc_l_tmp_class_field_load_result_n85, hxc_l_tmp_load_result_n86);
    hxc_l_gc_roots[37] = (const void *)hxc_l_tmp_managed_class_result_n87;
    int32_t hxc_l_tmp_array_push_result_n88;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n80, &(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeFluidDefinition *){ hxc_l_tmp_managed_class_result_n87 }, &hxc_l_tmp_array_push_result_n88) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n88;
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n29) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_audio) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n27) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n26) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_camera) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n24) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n23) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_render) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n21) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n20) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_simulation) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n18) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n17) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n15) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n12) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n10) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n9) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  hxc_l_gc_roots[13] = (const void *)hxc_l_result;
  bool hxc_l_tmp_call_result_n90 = hxc_caxecraft_content_RuntimeContentPack_validateFluidOrder(hxc_l_reader, (hxc_string){ (const uint8_t *)"fluids", 6, true, NULL }, hxc_l_result);
  if (!hxc_l_tmp_call_result_n90)
  {
    hxc_l_gc_roots[39] = (const void *)NULL;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  hxc_l_gc_roots[38] = (const void *)hxc_l_result;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_result;
}

struct hxc_array_ref *hxc_caxecraft_content_RuntimeContentPack_readIdArray(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, struct hxc_caxecraft_content_ContentJsonNode *hxc_l_node, hxc_string hxc_l_path, int32_t hxc_l_minimum, int32_t hxc_l_maximum)
{
  const void *hxc_l_gc_roots[20] = { (const void *)hxc_l_node, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 20, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_value = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n11 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n12 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n13 = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n14 = { 0 };
  if (hxc_l_reader == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeSchemaReader_array(hxc_l_reader, hxc_l_node, hxc_l_path, hxc_l_minimum, hxc_l_maximum);
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_instance_call_result_n0;
  struct hxc_array_ref *hxc_l_values = hxc_l_tmp_instance_call_result_n0;
  hxc_l_gc_roots[2] = (const void *)hxc_l_values;
  hxc_l_gc_roots[3] = (const void *)NULL;
  if (hxc_l_values == NULL)
  {
    hxc_l_gc_roots[4] = (const void *)NULL;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n2 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_array_70e85228_descriptor, (void **)&hxc_l_tmp_array_create_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_init_in_place(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeLocatedId *), _Alignof(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeLocatedId *), NULL, NULL, NULL, NULL }, hxc_l_tmp_array_create_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_array_create_result_n2;
  struct hxc_array_ref *hxc_l_result = hxc_l_tmp_array_create_result_n2;
  int32_t hxc_l_g = 0;
  hxc_l_gc_roots[6] = (const void *)hxc_l_values;
  int32_t hxc_l_tmp_array_length_result_n4;
  if (hxc_array_ref_length(hxc_l_values, &hxc_l_tmp_array_length_result_n4) != HXC_STATUS_OK)
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
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n7;
    struct hxc_array_ref *hxc_l_tmp_load_result_n8 = hxc_l_values;
    hxc_l_gc_roots[7] = (const void *)hxc_l_tmp_load_result_n8;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_array_get_result_n10;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n8, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n10) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[8] = (const void *)hxc_l_tmp_array_get_result_n10;
    hxc_string hxc_l_tmp_string_concat_result_n11 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)"[", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n11) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_left_owner_n11 = hxc_l_tmp_string_concat_result_n11;
    hxc_string hxc_l_tmp_string_concat_left_borrow_result_n12 = hxc_l_tmp_string_concat_left_owner_n11;
    hxc_string hxc_l_tmp_std_string_int_result_n14 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_from_int32(hxc_l_index, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n14) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_right_owner_n12 = hxc_l_tmp_std_string_int_result_n14;
    hxc_string hxc_l_tmp_string_concat_result_n16 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_borrow_result_n12, hxc_l_tmp_string_concat_right_owner_n12, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n16) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_left_owner_n13 = hxc_l_tmp_string_concat_result_n16;
    hxc_string hxc_l_tmp_string_concat_result_n18 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n13, (hxc_string){ (const uint8_t *)"]", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n18) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n14 = hxc_l_tmp_string_concat_result_n18;
    hxc_string hxc_l_tmp_call_result_n20 = hxc_caxecraft_content_RuntimeContentPack_readContentId(hxc_l_reader, hxc_l_tmp_array_get_result_n10, hxc_l_tmp_static_call_argument_2_owner_n14);
    hxc_l_value = hxc_l_tmp_call_result_n20;
    if (hxc_l_value.data == NULL)
    {
      hxc_l_gc_roots[10] = (const void *)NULL;
      if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n14) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n13) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n12) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n11) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n22 = hxc_l_result;
    hxc_l_gc_roots[11] = (const void *)hxc_l_tmp_load_result_n22;
    hxc_string hxc_l_tmp_load_result_n23 = hxc_l_value;
    struct hxc_array_ref *hxc_l_tmp_load_result_n24 = hxc_l_values;
    hxc_l_gc_roots[12] = (const void *)hxc_l_tmp_load_result_n24;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_array_get_result_n26;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n24, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n26) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[13] = (const void *)hxc_l_tmp_array_get_result_n26;
    if (hxc_l_tmp_array_get_result_n26 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n27 = (*hxc_l_tmp_array_get_result_n26).hxc_line;
    struct hxc_array_ref *hxc_l_tmp_load_result_n28 = hxc_l_values;
    hxc_l_gc_roots[14] = (const void *)hxc_l_tmp_load_result_n28;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_array_get_result_n30;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n28, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n30) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[15] = (const void *)hxc_l_tmp_array_get_result_n30;
    if (hxc_l_tmp_array_get_result_n30 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n31 = (*hxc_l_tmp_array_get_result_n30).hxc_column;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeLocatedId *hxc_l_tmp_managed_class_result_n32 = NULL;
    if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_RuntimeContentPack_RuntimeLocatedId_descriptor, (void **)&hxc_l_tmp_managed_class_result_n32) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[16] = (const void *)hxc_l_tmp_managed_class_result_n32;
    hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeLocatedId(hxc_l_tmp_managed_class_result_n32, hxc_l_tmp_load_result_n23, hxc_l_tmp_class_field_load_result_n27, hxc_l_tmp_class_field_load_result_n31);
    hxc_l_gc_roots[17] = (const void *)hxc_l_tmp_managed_class_result_n32;
    int32_t hxc_l_tmp_array_push_result_n33;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n22, &(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeLocatedId *){ hxc_l_tmp_managed_class_result_n32 }, &hxc_l_tmp_array_push_result_n33) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n33;
    if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n14) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n13) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n12) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n11) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  hxc_l_gc_roots[9] = (const void *)hxc_l_result;
  bool hxc_l_tmp_call_result_n35 = hxc_caxecraft_content_RuntimeContentPack_validateOrder(hxc_l_reader, hxc_l_node, hxc_l_path, hxc_l_result);
  if (!hxc_l_tmp_call_result_n35)
  {
    hxc_l_gc_roots[19] = (const void *)NULL;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  hxc_l_gc_roots[18] = (const void *)hxc_l_result;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_result;
}

struct hxc_array_ref *hxc_caxecraft_content_RuntimeContentPack_readItems(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, struct hxc_caxecraft_content_ContentJsonNode *hxc_l_node, struct hxc_caxecraft_content_RuntimeAssetInventory *hxc_l_assets)
{
  const void *hxc_l_gc_roots[45] = { (const void *)hxc_l_node, (const void *)hxc_l_assets, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 45, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_string_concat_left_owner_n10 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_instance_call_argument_2_owner_n12 = { 0 };
  hxc_string hxc_l_id = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n15 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n17 = { 0 };
  hxc_string hxc_l_useText = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n19 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_static_call_argument_3_owner_n20 = { 0 };
  struct hxc_optional_caxecraft_content_LocatedOptionalString hxc_l_placement = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n22 = { 0 };
  struct hxc_optional_caxecraft_content_LocatedOptionalString hxc_l_aquatic = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n24 = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n26 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n34 = { 0 };
  hxc_string hxc_l_path = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n9 = { 0 };
  if (hxc_l_reader == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeSchemaReader_array(hxc_l_reader, hxc_l_node, (hxc_string){ (const uint8_t *)"items", 5, true, NULL }, 0, 128);
  hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_instance_call_result_n0;
  struct hxc_array_ref *hxc_l_values = hxc_l_tmp_instance_call_result_n0;
  hxc_l_gc_roots[3] = (const void *)hxc_l_values;
  hxc_l_gc_roots[4] = (const void *)NULL;
  if (hxc_l_values == NULL)
  {
    hxc_l_gc_roots[5] = (const void *)NULL;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n2 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_array_fb75e297_descriptor, (void **)&hxc_l_tmp_array_create_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_init_in_place(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *), _Alignof(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *), NULL, NULL, NULL, NULL }, hxc_l_tmp_array_create_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[6] = (const void *)hxc_l_tmp_array_create_result_n2;
  struct hxc_array_ref *hxc_l_result = hxc_l_tmp_array_create_result_n2;
  int32_t hxc_l_g = 0;
  hxc_l_gc_roots[7] = (const void *)hxc_l_values;
  int32_t hxc_l_tmp_array_length_result_n4;
  if (hxc_array_ref_length(hxc_l_values, &hxc_l_tmp_array_length_result_n4) != HXC_STATUS_OK)
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
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n7;
    hxc_string hxc_l_tmp_std_string_int_result_n9 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_from_int32(hxc_l_index, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_right_owner_n9 = hxc_l_tmp_std_string_int_result_n9;
    hxc_string hxc_l_tmp_string_concat_result_n11 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"items[", 6, true, NULL }, hxc_l_tmp_string_concat_right_owner_n9, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n11) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_left_owner_n10 = hxc_l_tmp_string_concat_result_n11;
    hxc_string hxc_l_tmp_string_concat_result_n13 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n10, (hxc_string){ (const uint8_t *)"]", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n13) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_path = hxc_l_tmp_string_concat_result_n13;
    struct hxc_array_ref *hxc_l_tmp_load_result_n14 = hxc_l_values;
    hxc_l_gc_roots[8] = (const void *)hxc_l_tmp_load_result_n14;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_array_get_result_n16;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n14, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n16) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[9] = (const void *)hxc_l_tmp_array_get_result_n16;
    hxc_string hxc_l_tmp_load_result_n17 = hxc_l_path;
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n18 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n18) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"id", 2, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"maxStack", 8, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"useProfile", 10, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"placementBlock", 14, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"aquaticProfile", 14, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"icon", 4, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_2_owner_n12 = hxc_l_tmp_array_create_result_n18;
    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n20 = hxc_caxecraft_content_RuntimeSchemaReader_object(hxc_l_reader, hxc_l_tmp_array_get_result_n16, hxc_l_tmp_load_result_n17, hxc_l_tmp_instance_call_argument_2_owner_n12);
    hxc_l_gc_roots[10] = (const void *)hxc_l_tmp_instance_call_result_n20;
    struct hxc_array_ref *hxc_l_fields = hxc_l_tmp_instance_call_result_n20;
    hxc_l_gc_roots[11] = (const void *)hxc_l_fields;
    hxc_l_gc_roots[12] = (const void *)NULL;
    if (hxc_l_fields == NULL)
    {
      hxc_l_gc_roots[14] = (const void *)NULL;
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n12) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n10) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n9) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
    hxc_l_gc_roots[15] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n23 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"id", 2, true, NULL });
    hxc_l_gc_roots[16] = (const void *)hxc_l_tmp_instance_call_result_n23;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_idNode = hxc_l_tmp_instance_call_result_n23;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n24 = hxc_l_idNode;
    hxc_l_gc_roots[17] = (const void *)hxc_l_tmp_load_result_n24;
    hxc_string hxc_l_tmp_string_concat_result_n26 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".id", 3, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n26) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n15 = hxc_l_tmp_string_concat_result_n26;
    hxc_string hxc_l_tmp_call_result_n28 = hxc_caxecraft_content_RuntimeContentPack_readContentId(hxc_l_reader, hxc_l_tmp_load_result_n24, hxc_l_tmp_static_call_argument_2_owner_n15);
    hxc_l_id = hxc_l_tmp_call_result_n28;
    hxc_l_gc_roots[18] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n30 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"maxStack", 8, true, NULL });
    hxc_l_gc_roots[19] = (const void *)hxc_l_tmp_instance_call_result_n30;
    hxc_string hxc_l_tmp_string_concat_result_n32 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".maxStack", 9, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n32) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n17 = hxc_l_tmp_string_concat_result_n32;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_instance_call_result_n34 = hxc_caxecraft_content_RuntimeSchemaReader_integer(hxc_l_reader, hxc_l_tmp_instance_call_result_n30, hxc_l_tmp_instance_call_argument_1_owner_n17, 1, 64);
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_maximumStack = hxc_l_tmp_instance_call_result_n34;
    hxc_l_gc_roots[20] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n36 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"useProfile", 10, true, NULL });
    hxc_l_gc_roots[21] = (const void *)hxc_l_tmp_instance_call_result_n36;
    hxc_string hxc_l_tmp_string_concat_result_n38 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".useProfile", 11, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n38) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n19 = hxc_l_tmp_string_concat_result_n38;
    hxc_string hxc_l_tmp_static_call_argument_2_borrow_result_n39 = hxc_l_tmp_static_call_argument_2_owner_n19;
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n40 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n40) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n40->value, &(hxc_string){ (const uint8_t *)"consume-one-heart", 17, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n40->value, &(hxc_string){ (const uint8_t *)"equip-aquatic", 13, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n40->value, &(hxc_string){ (const uint8_t *)"haxeforge-tool", 14, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n40->value, &(hxc_string){ (const uint8_t *)"light-source", 12, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n40->value, &(hxc_string){ (const uint8_t *)"melee-sword", 11, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n40->value, &(hxc_string){ (const uint8_t *)"none", 4, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n40->value, &(hxc_string){ (const uint8_t *)"place-block", 11, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_3_owner_n20 = hxc_l_tmp_array_create_result_n40;
    hxc_string hxc_l_tmp_call_result_n42 = hxc_caxecraft_content_RuntimeContentPack_readClosed(hxc_l_reader, hxc_l_tmp_instance_call_result_n36, hxc_l_tmp_static_call_argument_2_borrow_result_n39, hxc_l_tmp_static_call_argument_3_owner_n20);
    hxc_l_useText = hxc_l_tmp_call_result_n42;
    hxc_l_gc_roots[22] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n44 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"placementBlock", 14, true, NULL });
    hxc_l_gc_roots[23] = (const void *)hxc_l_tmp_instance_call_result_n44;
    hxc_string hxc_l_tmp_string_concat_result_n46 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".placementBlock", 15, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n46) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n22 = hxc_l_tmp_string_concat_result_n46;
    struct hxc_optional_caxecraft_content_LocatedOptionalString hxc_l_tmp_instance_call_result_n48 = hxc_caxecraft_content_RuntimeSchemaReader_nullableContentId(hxc_l_reader, hxc_l_tmp_instance_call_result_n44, hxc_l_tmp_instance_call_argument_1_owner_n22);
    hxc_l_placement = hxc_l_tmp_instance_call_result_n48;
    hxc_l_gc_roots[24] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n50 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"aquaticProfile", 14, true, NULL });
    hxc_l_gc_roots[25] = (const void *)hxc_l_tmp_instance_call_result_n50;
    hxc_string hxc_l_tmp_string_concat_result_n52 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".aquaticProfile", 15, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n52) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n24 = hxc_l_tmp_string_concat_result_n52;
    struct hxc_optional_caxecraft_content_LocatedOptionalString hxc_l_tmp_instance_call_result_n54 = hxc_caxecraft_content_RuntimeSchemaReader_nullableContentId(hxc_l_reader, hxc_l_tmp_instance_call_result_n50, hxc_l_tmp_instance_call_argument_1_owner_n24);
    hxc_l_aquatic = hxc_l_tmp_instance_call_result_n54;
    hxc_l_gc_roots[26] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n56 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"icon", 4, true, NULL });
    hxc_l_gc_roots[27] = (const void *)hxc_l_tmp_instance_call_result_n56;
    hxc_string hxc_l_tmp_string_concat_result_n58 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".icon", 5, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n58) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n26 = hxc_l_tmp_string_concat_result_n58;
    struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_call_result_n60 = hxc_caxecraft_content_RuntimeContentPack_readPresentation(hxc_l_reader, hxc_l_tmp_instance_call_result_n56, hxc_l_tmp_static_call_argument_2_owner_n26, hxc_l_assets);
    hxc_l_gc_roots[28] = (const void *)hxc_l_tmp_call_result_n60;
    struct hxc_caxecraft_content_RuntimePresentation *hxc_l_presentation = hxc_l_tmp_call_result_n60;
    struct hxc_optional_caxecraft_content_RuntimeSchemaDiagnostic hxc_l_tmp_class_field_load_result_n61 = (*hxc_l_reader).hxc_failure;
    bool hxc_l_tmp_short_circuit_result_n27 = hxc_l_tmp_class_field_load_result_n61.hxc_has_value;
    if (!hxc_l_tmp_class_field_load_result_n61.hxc_has_value)
    {
      hxc_l_tmp_short_circuit_result_n27 = hxc_l_id.data == NULL;
    }
    bool hxc_l_tmp_short_circuit_load_result_n63 = hxc_l_tmp_short_circuit_result_n27;
    bool hxc_l_tmp_short_circuit_result_n28 = hxc_l_tmp_short_circuit_load_result_n63;
    if (!hxc_l_tmp_short_circuit_load_result_n63)
    {
      hxc_l_tmp_short_circuit_result_n28 = !hxc_l_maximumStack.hxc_has_value;
    }
    bool hxc_l_tmp_short_circuit_load_result_n65 = hxc_l_tmp_short_circuit_result_n28;
    bool hxc_l_tmp_short_circuit_result_n29 = hxc_l_tmp_short_circuit_load_result_n65;
    if (!hxc_l_tmp_short_circuit_load_result_n65)
    {
      hxc_l_tmp_short_circuit_result_n29 = hxc_l_useText.data == NULL;
    }
    bool hxc_l_tmp_short_circuit_load_result_n67 = hxc_l_tmp_short_circuit_result_n29;
    bool hxc_l_tmp_short_circuit_result_n30 = hxc_l_tmp_short_circuit_load_result_n67;
    if (!hxc_l_tmp_short_circuit_load_result_n67)
    {
      hxc_l_tmp_short_circuit_result_n30 = !hxc_l_placement.hxc_has_value;
    }
    bool hxc_l_tmp_short_circuit_load_result_n69 = hxc_l_tmp_short_circuit_result_n30;
    bool hxc_l_tmp_short_circuit_result_n31 = hxc_l_tmp_short_circuit_load_result_n69;
    if (!hxc_l_tmp_short_circuit_load_result_n69)
    {
      hxc_l_tmp_short_circuit_result_n31 = !hxc_l_aquatic.hxc_has_value;
    }
    bool hxc_l_tmp_short_circuit_load_result_n71 = hxc_l_tmp_short_circuit_result_n31;
    bool hxc_l_tmp_short_circuit_result_n32 = hxc_l_tmp_short_circuit_load_result_n71;
    if (!hxc_l_tmp_short_circuit_load_result_n71)
    {
      hxc_l_gc_roots[29] = (const void *)hxc_l_presentation;
      hxc_l_gc_roots[30] = (const void *)NULL;
      hxc_l_tmp_short_circuit_result_n32 = hxc_l_presentation == NULL;
    }
    if (hxc_l_tmp_short_circuit_result_n32)
    {
      hxc_l_gc_roots[31] = (const void *)NULL;
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n26) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_optional_01d9fa01_destroy(&hxc_l_aquatic);
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n24) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_optional_01d9fa01_destroy(&hxc_l_placement);
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n22) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_useText) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n20) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n19) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n17) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n15) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n12) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n10) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n9) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
    hxc_string hxc_l_tmp_load_result_n74 = hxc_l_useText;
    struct hxc_optional_caxecraft_content_LocatedOptionalString hxc_l_tmp_load_result_n76 = hxc_l_placement;
    if (!hxc_l_tmp_load_result_n76.hxc_has_value)
    {
      abort();
    }
    bool hxc_l_tmp_short_circuit_result_n33 = (hxc_l_tmp_load_result_n74.data != NULL && (hxc_l_tmp_load_result_n74.byte_length == (hxc_string){ (const uint8_t *)"place-block", 11, true, NULL }.byte_length && (hxc_l_tmp_load_result_n74.byte_length == 0 || memcmp(hxc_l_tmp_load_result_n74.data, (hxc_string){ (const uint8_t *)"place-block", 11, true, NULL }.data, hxc_l_tmp_load_result_n74.byte_length) == 0))) != (hxc_l_tmp_load_result_n76.hxc_value.hxc_value.data != NULL);
    if (!((hxc_l_tmp_load_result_n74.data != NULL && (hxc_l_tmp_load_result_n74.byte_length == (hxc_string){ (const uint8_t *)"place-block", 11, true, NULL }.byte_length && (hxc_l_tmp_load_result_n74.byte_length == 0 || memcmp(hxc_l_tmp_load_result_n74.data, (hxc_string){ (const uint8_t *)"place-block", 11, true, NULL }.data, hxc_l_tmp_load_result_n74.byte_length) == 0))) != (hxc_l_tmp_load_result_n76.hxc_value.hxc_value.data != NULL)))
    {
      hxc_string hxc_l_tmp_load_result_n79 = hxc_l_useText;
      struct hxc_optional_caxecraft_content_LocatedOptionalString hxc_l_tmp_load_result_n81 = hxc_l_aquatic;
      if (!hxc_l_tmp_load_result_n81.hxc_has_value)
      {
        abort();
      }
      hxc_l_tmp_short_circuit_result_n33 = (hxc_l_tmp_load_result_n79.data != NULL && (hxc_l_tmp_load_result_n79.byte_length == (hxc_string){ (const uint8_t *)"equip-aquatic", 13, true, NULL }.byte_length && (hxc_l_tmp_load_result_n79.byte_length == 0 || memcmp(hxc_l_tmp_load_result_n79.data, (hxc_string){ (const uint8_t *)"equip-aquatic", 13, true, NULL }.data, hxc_l_tmp_load_result_n79.byte_length) == 0))) != (hxc_l_tmp_load_result_n81.hxc_value.hxc_value.data != NULL);
    }
    if (hxc_l_tmp_short_circuit_result_n33)
    {
      struct hxc_array_ref *hxc_l_tmp_load_result_n85 = hxc_l_values;
      hxc_l_gc_roots[32] = (const void *)hxc_l_tmp_load_result_n85;
      struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_array_get_result_n87;
      if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n85, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n87) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_gc_roots[33] = (const void *)hxc_l_tmp_array_get_result_n87;
      hxc_string hxc_l_tmp_string_concat_result_n89 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".useProfile", 11, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n89) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_instance_call_argument_1_owner_n34 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidInvariant, .hxc_payload.hxc_SchemaInvalidInvariant.hxc_path = hxc_l_tmp_string_concat_result_n89 };
      hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_reader, hxc_l_tmp_array_get_result_n87, hxc_l_tmp_instance_call_argument_1_owner_n34);
      hxc_l_gc_roots[34] = (const void *)NULL;
      hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n34);
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n26) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_optional_01d9fa01_destroy(&hxc_l_aquatic);
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n24) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_optional_01d9fa01_destroy(&hxc_l_placement);
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n22) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_useText) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n20) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n19) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n17) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n15) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n12) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n10) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n9) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n92 = hxc_l_result;
    hxc_l_gc_roots[35] = (const void *)hxc_l_tmp_load_result_n92;
    hxc_string hxc_l_tmp_load_result_n93 = hxc_l_id;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n94 = hxc_l_idNode;
    hxc_l_gc_roots[36] = (const void *)hxc_l_tmp_load_result_n94;
    if (hxc_l_tmp_load_result_n94 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n95 = (*hxc_l_tmp_load_result_n94).hxc_line;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n96 = hxc_l_idNode;
    hxc_l_gc_roots[37] = (const void *)hxc_l_tmp_load_result_n96;
    if (hxc_l_tmp_load_result_n96 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n97 = (*hxc_l_tmp_load_result_n96).hxc_column;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n98 = hxc_l_maximumStack;
    if (!hxc_l_tmp_load_result_n98.hxc_has_value)
    {
      abort();
    }
    enum hxc_caxecraft_content_RuntimeItemUseProfile hxc_l_tmp_call_result_n101 = hxc_caxecraft_content_RuntimeContentPack_itemUse(hxc_l_useText);
    struct hxc_optional_caxecraft_content_LocatedOptionalString hxc_l_tmp_load_result_n102 = hxc_l_placement;
    if (!hxc_l_tmp_load_result_n102.hxc_has_value)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference hxc_l_tmp_call_result_n104 = hxc_caxecraft_content_RuntimeContentPack_optionalReference(hxc_l_tmp_load_result_n102.hxc_value);
    hxc_l_gc_roots[38] = hxc_l_tmp_call_result_n104.hxc_tag == hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady ? (const void *)hxc_l_tmp_call_result_n104.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference : NULL;
    struct hxc_optional_caxecraft_content_LocatedOptionalString hxc_l_tmp_load_result_n105 = hxc_l_aquatic;
    if (!hxc_l_tmp_load_result_n105.hxc_has_value)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference hxc_l_tmp_call_result_n107 = hxc_caxecraft_content_RuntimeContentPack_optionalReference(hxc_l_tmp_load_result_n105.hxc_value);
    hxc_l_gc_roots[39] = hxc_l_tmp_call_result_n107.hxc_tag == hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady ? (const void *)hxc_l_tmp_call_result_n107.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference : NULL;
    struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_load_result_n108 = hxc_l_presentation;
    hxc_l_gc_roots[40] = (const void *)hxc_l_tmp_load_result_n108;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_tmp_managed_class_result_n109 = NULL;
    if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition_descriptor, (void **)&hxc_l_tmp_managed_class_result_n109) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[41] = (const void *)hxc_l_tmp_managed_class_result_n109;
    hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition(hxc_l_tmp_managed_class_result_n109, hxc_l_tmp_load_result_n93, hxc_l_tmp_class_field_load_result_n95, hxc_l_tmp_class_field_load_result_n97, hxc_l_tmp_load_result_n98.hxc_value, hxc_l_tmp_call_result_n101, hxc_l_tmp_call_result_n104, hxc_l_tmp_call_result_n107, hxc_l_tmp_load_result_n108);
    hxc_l_gc_roots[42] = (const void *)hxc_l_tmp_managed_class_result_n109;
    int32_t hxc_l_tmp_array_push_result_n110;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n92, &(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *){ hxc_l_tmp_managed_class_result_n109 }, &hxc_l_tmp_array_push_result_n110) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n110;
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n26) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_01d9fa01_destroy(&hxc_l_aquatic);
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n24) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_01d9fa01_destroy(&hxc_l_placement);
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n22) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_useText) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n20) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n19) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n17) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n15) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n12) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n10) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n9) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  hxc_l_gc_roots[13] = (const void *)hxc_l_result;
  bool hxc_l_tmp_call_result_n112 = hxc_caxecraft_content_RuntimeContentPack_validateItemOrder(hxc_l_reader, (hxc_string){ (const uint8_t *)"items", 5, true, NULL }, hxc_l_result);
  if (!hxc_l_tmp_call_result_n112)
  {
    hxc_l_gc_roots[44] = (const void *)NULL;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  hxc_l_gc_roots[43] = (const void *)hxc_l_result;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_result;
}

hxc_string hxc_caxecraft_content_RuntimeContentPack_readLogicalPath(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, struct hxc_caxecraft_content_ContentJsonNode *hxc_l_node, hxc_string hxc_l_path)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_node };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_returned_string_owner_n4 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n6 = { 0 };
  if (hxc_l_reader == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeSchemaReader_string(hxc_l_reader, hxc_l_node, hxc_l_path, 128);
  hxc_string hxc_l_value = hxc_l_tmp_instance_call_result_n0;
  if (hxc_l_value.data == NULL)
  {
    hxc_l_tmp_returned_string_owner_n4 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n4) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_string hxc_l_tmp_returned_string_owned_load_result_n2 = hxc_l_tmp_returned_string_owner_n4;
    if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_returned_string_owned_load_result_n2;
  }
  bool hxc_l_tmp_call_result_n4 = hxc_caxecraft_content_RuntimeSchemaReader_validLogicalPath(hxc_l_value);
  if (!!hxc_l_tmp_call_result_n4)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_value;
  }
  hxc_string hxc_l_tmp_enum_payload_0_owner_n5 = hxc_l_path;
  if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_instance_call_argument_1_owner_n6 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidLogicalPath, .hxc_payload.hxc_SchemaInvalidLogicalPath.hxc_path = hxc_l_tmp_enum_payload_0_owner_n5 };
  hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_reader, hxc_l_node, hxc_l_tmp_instance_call_argument_1_owner_n6);
  hxc_string hxc_l_tmp_returned_string_owner_n7 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n7) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_returned_string_owned_load_result_n8 = hxc_l_tmp_returned_string_owner_n7;
  hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n6);
  if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_returned_string_owned_load_result_n8;
}

struct hxc_array_ref *hxc_caxecraft_content_RuntimeContentPack_readNpcs(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, struct hxc_caxecraft_content_ContentJsonNode *hxc_l_node, struct hxc_caxecraft_content_RuntimeAssetInventory *hxc_l_assets)
{
  const void *hxc_l_gc_roots[38] = { (const void *)hxc_l_node, (const void *)hxc_l_assets, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 38, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_string_concat_left_owner_n10 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_instance_call_argument_2_owner_n12 = { 0 };
  hxc_string hxc_l_id = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n15 = { 0 };
  hxc_string hxc_l_behavior = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n17 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_static_call_argument_3_owner_n18 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n20 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n22 = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n24 = { 0 };
  hxc_string hxc_l_path = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n9 = { 0 };
  if (hxc_l_reader == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeSchemaReader_array(hxc_l_reader, hxc_l_node, (hxc_string){ (const uint8_t *)"npcs", 4, true, NULL }, 0, 128);
  hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_instance_call_result_n0;
  struct hxc_array_ref *hxc_l_values = hxc_l_tmp_instance_call_result_n0;
  hxc_l_gc_roots[3] = (const void *)hxc_l_values;
  hxc_l_gc_roots[4] = (const void *)NULL;
  if (hxc_l_values == NULL)
  {
    hxc_l_gc_roots[5] = (const void *)NULL;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n2 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_array_1c332922_descriptor, (void **)&hxc_l_tmp_array_create_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_init_in_place(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeNpcDefinition *), _Alignof(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeNpcDefinition *), NULL, NULL, NULL, NULL }, hxc_l_tmp_array_create_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[6] = (const void *)hxc_l_tmp_array_create_result_n2;
  struct hxc_array_ref *hxc_l_result = hxc_l_tmp_array_create_result_n2;
  int32_t hxc_l_g = 0;
  hxc_l_gc_roots[7] = (const void *)hxc_l_values;
  int32_t hxc_l_tmp_array_length_result_n4;
  if (hxc_array_ref_length(hxc_l_values, &hxc_l_tmp_array_length_result_n4) != HXC_STATUS_OK)
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
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n7;
    hxc_string hxc_l_tmp_std_string_int_result_n9 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_from_int32(hxc_l_index, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_right_owner_n9 = hxc_l_tmp_std_string_int_result_n9;
    hxc_string hxc_l_tmp_string_concat_result_n11 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"npcs[", 5, true, NULL }, hxc_l_tmp_string_concat_right_owner_n9, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n11) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_left_owner_n10 = hxc_l_tmp_string_concat_result_n11;
    hxc_string hxc_l_tmp_string_concat_result_n13 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n10, (hxc_string){ (const uint8_t *)"]", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n13) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_path = hxc_l_tmp_string_concat_result_n13;
    struct hxc_array_ref *hxc_l_tmp_load_result_n14 = hxc_l_values;
    hxc_l_gc_roots[8] = (const void *)hxc_l_tmp_load_result_n14;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_array_get_result_n16;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n14, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n16) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[9] = (const void *)hxc_l_tmp_array_get_result_n16;
    hxc_string hxc_l_tmp_load_result_n17 = hxc_l_path;
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n18 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n18) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"id", 2, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"behaviorProfile", 15, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"maxHealth", 9, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"interactionRadiusMilli", 22, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n18->value, &(hxc_string){ (const uint8_t *)"presentation", 12, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_2_owner_n12 = hxc_l_tmp_array_create_result_n18;
    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n20 = hxc_caxecraft_content_RuntimeSchemaReader_object(hxc_l_reader, hxc_l_tmp_array_get_result_n16, hxc_l_tmp_load_result_n17, hxc_l_tmp_instance_call_argument_2_owner_n12);
    hxc_l_gc_roots[10] = (const void *)hxc_l_tmp_instance_call_result_n20;
    struct hxc_array_ref *hxc_l_fields = hxc_l_tmp_instance_call_result_n20;
    hxc_l_gc_roots[11] = (const void *)hxc_l_fields;
    hxc_l_gc_roots[12] = (const void *)NULL;
    if (hxc_l_fields == NULL)
    {
      hxc_l_gc_roots[14] = (const void *)NULL;
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n12) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n10) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n9) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
    hxc_l_gc_roots[15] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n23 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"id", 2, true, NULL });
    hxc_l_gc_roots[16] = (const void *)hxc_l_tmp_instance_call_result_n23;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_idNode = hxc_l_tmp_instance_call_result_n23;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n24 = hxc_l_idNode;
    hxc_l_gc_roots[17] = (const void *)hxc_l_tmp_load_result_n24;
    hxc_string hxc_l_tmp_string_concat_result_n26 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".id", 3, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n26) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n15 = hxc_l_tmp_string_concat_result_n26;
    hxc_string hxc_l_tmp_call_result_n28 = hxc_caxecraft_content_RuntimeContentPack_readContentId(hxc_l_reader, hxc_l_tmp_load_result_n24, hxc_l_tmp_static_call_argument_2_owner_n15);
    hxc_l_id = hxc_l_tmp_call_result_n28;
    hxc_l_gc_roots[18] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n30 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"behaviorProfile", 15, true, NULL });
    hxc_l_gc_roots[19] = (const void *)hxc_l_tmp_instance_call_result_n30;
    hxc_string hxc_l_tmp_string_concat_result_n32 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".behaviorProfile", 16, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n32) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n17 = hxc_l_tmp_string_concat_result_n32;
    hxc_string hxc_l_tmp_static_call_argument_2_borrow_result_n33 = hxc_l_tmp_static_call_argument_2_owner_n17;
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n34 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n34) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n34->value, &(hxc_string){ (const uint8_t *)"stationary-dialogue", 19, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_3_owner_n18 = hxc_l_tmp_array_create_result_n34;
    hxc_string hxc_l_tmp_call_result_n36 = hxc_caxecraft_content_RuntimeContentPack_readClosed(hxc_l_reader, hxc_l_tmp_instance_call_result_n30, hxc_l_tmp_static_call_argument_2_borrow_result_n33, hxc_l_tmp_static_call_argument_3_owner_n18);
    hxc_l_behavior = hxc_l_tmp_call_result_n36;
    hxc_l_gc_roots[20] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n38 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"maxHealth", 9, true, NULL });
    hxc_l_gc_roots[21] = (const void *)hxc_l_tmp_instance_call_result_n38;
    hxc_string hxc_l_tmp_string_concat_result_n40 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".maxHealth", 10, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n40) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n20 = hxc_l_tmp_string_concat_result_n40;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_instance_call_result_n42 = hxc_caxecraft_content_RuntimeSchemaReader_integer(hxc_l_reader, hxc_l_tmp_instance_call_result_n38, hxc_l_tmp_instance_call_argument_1_owner_n20, 1, 10000);
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_health = hxc_l_tmp_instance_call_result_n42;
    hxc_l_gc_roots[22] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n44 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"interactionRadiusMilli", 22, true, NULL });
    hxc_l_gc_roots[23] = (const void *)hxc_l_tmp_instance_call_result_n44;
    hxc_string hxc_l_tmp_string_concat_result_n46 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".interactionRadiusMilli", 23, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n46) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n22 = hxc_l_tmp_string_concat_result_n46;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_instance_call_result_n48 = hxc_caxecraft_content_RuntimeSchemaReader_integer(hxc_l_reader, hxc_l_tmp_instance_call_result_n44, hxc_l_tmp_instance_call_argument_1_owner_n22, 250, 32000);
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_radius = hxc_l_tmp_instance_call_result_n48;
    hxc_l_gc_roots[24] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n50 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"presentation", 12, true, NULL });
    hxc_l_gc_roots[25] = (const void *)hxc_l_tmp_instance_call_result_n50;
    hxc_string hxc_l_tmp_string_concat_result_n52 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".presentation", 13, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n52) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n24 = hxc_l_tmp_string_concat_result_n52;
    struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_call_result_n54 = hxc_caxecraft_content_RuntimeContentPack_readActorPresentation(hxc_l_reader, hxc_l_tmp_instance_call_result_n50, hxc_l_tmp_static_call_argument_2_owner_n24, hxc_l_assets);
    hxc_l_gc_roots[26] = (const void *)hxc_l_tmp_call_result_n54;
    struct hxc_caxecraft_content_RuntimePresentation *hxc_l_presentation = hxc_l_tmp_call_result_n54;
    struct hxc_optional_caxecraft_content_RuntimeSchemaDiagnostic hxc_l_tmp_class_field_load_result_n55 = (*hxc_l_reader).hxc_failure;
    bool hxc_l_tmp_short_circuit_result_n25 = hxc_l_tmp_class_field_load_result_n55.hxc_has_value;
    if (!hxc_l_tmp_class_field_load_result_n55.hxc_has_value)
    {
      hxc_l_tmp_short_circuit_result_n25 = hxc_l_id.data == NULL;
    }
    bool hxc_l_tmp_short_circuit_load_result_n57 = hxc_l_tmp_short_circuit_result_n25;
    bool hxc_l_tmp_short_circuit_result_n26 = hxc_l_tmp_short_circuit_load_result_n57;
    if (!hxc_l_tmp_short_circuit_load_result_n57)
    {
      hxc_l_tmp_short_circuit_result_n26 = hxc_l_behavior.data == NULL;
    }
    bool hxc_l_tmp_short_circuit_load_result_n59 = hxc_l_tmp_short_circuit_result_n26;
    bool hxc_l_tmp_short_circuit_result_n27 = hxc_l_tmp_short_circuit_load_result_n59;
    if (!hxc_l_tmp_short_circuit_load_result_n59)
    {
      hxc_l_tmp_short_circuit_result_n27 = !hxc_l_health.hxc_has_value;
    }
    bool hxc_l_tmp_short_circuit_load_result_n61 = hxc_l_tmp_short_circuit_result_n27;
    bool hxc_l_tmp_short_circuit_result_n28 = hxc_l_tmp_short_circuit_load_result_n61;
    if (!hxc_l_tmp_short_circuit_load_result_n61)
    {
      hxc_l_tmp_short_circuit_result_n28 = !hxc_l_radius.hxc_has_value;
    }
    bool hxc_l_tmp_short_circuit_load_result_n63 = hxc_l_tmp_short_circuit_result_n28;
    bool hxc_l_tmp_short_circuit_result_n29 = hxc_l_tmp_short_circuit_load_result_n63;
    if (!hxc_l_tmp_short_circuit_load_result_n63)
    {
      hxc_l_gc_roots[27] = (const void *)hxc_l_presentation;
      hxc_l_gc_roots[28] = (const void *)NULL;
      hxc_l_tmp_short_circuit_result_n29 = hxc_l_presentation == NULL;
    }
    if (hxc_l_tmp_short_circuit_result_n29)
    {
      hxc_l_gc_roots[29] = (const void *)NULL;
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n24) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n22) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n20) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_behavior) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n18) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n17) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n15) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n12) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n10) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n9) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n66 = hxc_l_result;
    hxc_l_gc_roots[30] = (const void *)hxc_l_tmp_load_result_n66;
    hxc_string hxc_l_tmp_load_result_n67 = hxc_l_id;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n68 = hxc_l_idNode;
    hxc_l_gc_roots[31] = (const void *)hxc_l_tmp_load_result_n68;
    if (hxc_l_tmp_load_result_n68 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n69 = (*hxc_l_tmp_load_result_n68).hxc_line;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n70 = hxc_l_idNode;
    hxc_l_gc_roots[32] = (const void *)hxc_l_tmp_load_result_n70;
    if (hxc_l_tmp_load_result_n70 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n71 = (*hxc_l_tmp_load_result_n70).hxc_column;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n72 = hxc_l_health;
    if (!hxc_l_tmp_load_result_n72.hxc_has_value)
    {
      abort();
    }
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_load_result_n74 = hxc_l_radius;
    if (!hxc_l_tmp_load_result_n74.hxc_has_value)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_load_result_n76 = hxc_l_presentation;
    hxc_l_gc_roots[33] = (const void *)hxc_l_tmp_load_result_n76;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeNpcDefinition *hxc_l_tmp_managed_class_result_n77 = NULL;
    if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_RuntimeContentPack_RuntimeNpcDefinition_descriptor, (void **)&hxc_l_tmp_managed_class_result_n77) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[34] = (const void *)hxc_l_tmp_managed_class_result_n77;
    hxc_compiler_constructor_caxecraft_content_RuntimeContentPack_RuntimeNpcDefinition(hxc_l_tmp_managed_class_result_n77, hxc_l_tmp_load_result_n67, hxc_l_tmp_class_field_load_result_n69, hxc_l_tmp_class_field_load_result_n71, hxc_l_tmp_load_result_n72.hxc_value, hxc_l_tmp_load_result_n74.hxc_value, hxc_l_tmp_load_result_n76);
    hxc_l_gc_roots[35] = (const void *)hxc_l_tmp_managed_class_result_n77;
    int32_t hxc_l_tmp_array_push_result_n78;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n66, &(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeNpcDefinition *){ hxc_l_tmp_managed_class_result_n77 }, &hxc_l_tmp_array_push_result_n78) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n78;
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n24) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n22) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n20) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_behavior) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_3_owner_n18) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n17) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n15) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n12) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n10) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n9) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  hxc_l_gc_roots[13] = (const void *)hxc_l_result;
  bool hxc_l_tmp_call_result_n80 = hxc_caxecraft_content_RuntimeContentPack_validateNpcOrder(hxc_l_reader, (hxc_string){ (const uint8_t *)"npcs", 4, true, NULL }, hxc_l_result);
  if (!hxc_l_tmp_call_result_n80)
  {
    hxc_l_gc_roots[37] = (const void *)NULL;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  hxc_l_gc_roots[36] = (const void *)hxc_l_result;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_result;
}

struct hxc_caxecraft_content_RuntimePresentation *hxc_caxecraft_content_RuntimeContentPack_readPresentation(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, struct hxc_caxecraft_content_ContentJsonNode *hxc_l_node, hxc_string hxc_l_path, struct hxc_caxecraft_content_RuntimeAssetInventory *hxc_l_assets)
{
  const void *hxc_l_gc_roots[19] = { (const void *)hxc_l_node, (const void *)hxc_l_assets, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 19, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_cell = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n11 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_1_owner_n13 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n14 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n18 = { 0 };
  hxc_string hxc_l_asset = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n9 = { 0 };
  if (hxc_l_reader == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n0 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"asset", 5, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"cell", 4, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_argument_2_owner_n5 = hxc_l_tmp_array_create_result_n0;
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n2 = hxc_caxecraft_content_RuntimeSchemaReader_object(hxc_l_reader, hxc_l_node, hxc_l_path, hxc_l_tmp_instance_call_argument_2_owner_n5);
  hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_instance_call_result_n2;
  struct hxc_array_ref *hxc_l_fields = hxc_l_tmp_instance_call_result_n2;
  hxc_l_gc_roots[3] = (const void *)hxc_l_fields;
  hxc_l_gc_roots[4] = (const void *)NULL;
  if (hxc_l_fields == NULL)
  {
    hxc_l_gc_roots[5] = (const void *)NULL;
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  hxc_l_gc_roots[6] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n5 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"asset", 5, true, NULL });
  hxc_l_gc_roots[7] = (const void *)hxc_l_tmp_instance_call_result_n5;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_assetNode = hxc_l_tmp_instance_call_result_n5;
  hxc_l_gc_roots[8] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n7 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"cell", 4, true, NULL });
  hxc_l_gc_roots[9] = (const void *)hxc_l_tmp_instance_call_result_n7;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_cellNode = hxc_l_tmp_instance_call_result_n7;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n8 = hxc_l_assetNode;
  hxc_l_gc_roots[10] = (const void *)hxc_l_tmp_load_result_n8;
  hxc_string hxc_l_tmp_string_concat_result_n9 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".asset", 6, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n9) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_instance_call_argument_1_owner_n9 = hxc_l_tmp_string_concat_result_n9;
  hxc_string hxc_l_tmp_instance_call_result_n11 = hxc_caxecraft_content_RuntimeSchemaReader_string(hxc_l_reader, hxc_l_tmp_load_result_n8, hxc_l_tmp_instance_call_argument_1_owner_n9, 128);
  hxc_l_asset = hxc_l_tmp_instance_call_result_n11;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n12 = hxc_l_cellNode;
  hxc_l_gc_roots[11] = (const void *)hxc_l_tmp_load_result_n12;
  hxc_string hxc_l_tmp_string_concat_result_n13 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".cell", 5, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n13) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_instance_call_argument_1_owner_n11 = hxc_l_tmp_string_concat_result_n13;
  hxc_string hxc_l_tmp_instance_call_result_n15 = hxc_caxecraft_content_RuntimeSchemaReader_string(hxc_l_reader, hxc_l_tmp_load_result_n12, hxc_l_tmp_instance_call_argument_1_owner_n11, 128);
  hxc_l_cell = hxc_l_tmp_instance_call_result_n15;
  hxc_string hxc_l_tmp_load_result_n16 = hxc_l_asset;
  bool hxc_l_tmp_short_circuit_result_n12 = hxc_l_tmp_load_result_n16.data == NULL;
  if (!(hxc_l_tmp_load_result_n16.data == NULL))
  {
    hxc_l_tmp_short_circuit_result_n12 = hxc_l_cell.data == NULL;
  }
  if (hxc_l_tmp_short_circuit_result_n12)
  {
    hxc_l_gc_roots[12] = (const void *)NULL;
    if (hxc_string_release(&hxc_l_cell) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n11) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_asset) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  if (hxc_l_assets == NULL)
  {
    abort();
  }
  bool hxc_l_tmp_instance_call_result_n20 = hxc_caxecraft_content_RuntimeAssetInventory_hasAsset(hxc_l_assets, hxc_l_asset);
  if (!hxc_l_tmp_instance_call_result_n20)
  {
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n21 = hxc_l_assetNode;
    hxc_l_gc_roots[13] = (const void *)hxc_l_tmp_load_result_n21;
    hxc_string hxc_l_tmp_string_concat_result_n22 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".asset", 6, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n22) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_enum_payload_1_owner_n13 = hxc_l_asset;
    if (hxc_string_retain(hxc_l_tmp_enum_payload_1_owner_n13) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n14 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaUnknownAsset, .hxc_payload.hxc_SchemaUnknownAsset.hxc_path = hxc_l_tmp_string_concat_result_n22, .hxc_payload.hxc_SchemaUnknownAsset.hxc_id = hxc_l_tmp_enum_payload_1_owner_n13 };
    hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_reader, hxc_l_tmp_load_result_n21, hxc_l_tmp_instance_call_argument_1_owner_n14);
    hxc_l_gc_roots[14] = (const void *)NULL;
    hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n14);
    if (hxc_string_release(&hxc_l_cell) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n11) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_asset) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  hxc_string hxc_l_tmp_load_result_n27 = hxc_l_asset;
  int32_t hxc_l_tmp_instance_call_result_n29 = hxc_caxecraft_content_RuntimeAssetInventory_cellIndex(hxc_l_assets, hxc_l_tmp_load_result_n27, hxc_l_cell);
  int32_t hxc_l_index = hxc_l_tmp_instance_call_result_n29;
  if (!(hxc_l_index < 0))
  {
    hxc_string hxc_l_tmp_load_result_n39 = hxc_l_asset;
    hxc_string hxc_l_tmp_load_result_n40 = hxc_l_cell;
    int32_t hxc_l_tmp_load_result_n41 = hxc_l_index;
    struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_managed_class_result_n42 = NULL;
    if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_content_RuntimePresentation_descriptor, (void **)&hxc_l_tmp_managed_class_result_n42) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[17] = (const void *)hxc_l_tmp_managed_class_result_n42;
    hxc_compiler_constructor_caxecraft_content_RuntimePresentation(hxc_l_tmp_managed_class_result_n42, hxc_l_tmp_load_result_n39, hxc_l_tmp_load_result_n40, hxc_l_tmp_load_result_n41);
    hxc_l_gc_roots[18] = (const void *)hxc_l_tmp_managed_class_result_n42;
    if (hxc_string_release(&hxc_l_cell) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n11) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_asset) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_managed_class_result_n42;
  }
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n31 = hxc_l_cellNode;
  hxc_l_gc_roots[15] = (const void *)hxc_l_tmp_load_result_n31;
  hxc_string hxc_l_tmp_string_concat_result_n32 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".cell", 5, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n32) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_enum_payload_1_owner_n16 = hxc_l_asset;
  if (hxc_string_retain(hxc_l_tmp_enum_payload_1_owner_n16) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_enum_payload_1_owned_load_result_n34 = hxc_l_tmp_enum_payload_1_owner_n16;
  hxc_string hxc_l_tmp_enum_payload_2_owner_n17 = hxc_l_cell;
  if (hxc_string_retain(hxc_l_tmp_enum_payload_2_owner_n17) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_instance_call_argument_1_owner_n18 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaUnknownAssetCell, .hxc_payload.hxc_SchemaUnknownAssetCell.hxc_path = hxc_l_tmp_string_concat_result_n32, .hxc_payload.hxc_SchemaUnknownAssetCell.hxc_asset = hxc_l_tmp_enum_payload_1_owned_load_result_n34, .hxc_payload.hxc_SchemaUnknownAssetCell.hxc_cell = hxc_l_tmp_enum_payload_2_owner_n17 };
  hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_reader, hxc_l_tmp_load_result_n31, hxc_l_tmp_instance_call_argument_1_owner_n18);
  hxc_l_gc_roots[16] = (const void *)NULL;
  hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n18);
  if (hxc_string_release(&hxc_l_cell) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n11) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_asset) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n9) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return NULL;
}

bool hxc_caxecraft_content_RuntimeContentPack_rejectReference(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_reference, hxc_string hxc_l_path, hxc_string hxc_l_expectedKind, struct hxc_array_ref *hxc_l_kinds)
{
  const void *hxc_l_gc_roots[5] = { (const void *)hxc_l_reference, (const void *)hxc_l_kinds, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 5, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_2_owner_n10 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_2_owner_n14 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n7 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_1_owner_n8 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_2_owner_n9 = { 0 };
  int32_t hxc_l_g = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n0 = hxc_l_g;
    int32_t hxc_l_tmp_array_length_result_n1;
    if (hxc_array_ref_length(hxc_l_kinds, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n0 < hxc_l_tmp_array_length_result_n1))
    {
      break;
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeKindId *hxc_l_tmp_array_get_result_n3;
    if (hxc_array_ref_get_copy(hxc_l_kinds, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_array_get_result_n3;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeKindId *hxc_l_entry = hxc_l_tmp_array_get_result_n3;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeKindId *hxc_l_tmp_load_result_n5 = hxc_l_entry;
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_load_result_n5;
    if (hxc_l_tmp_load_result_n5 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n6 = (*hxc_l_tmp_load_result_n5).hxc_id;
    if (hxc_l_reference == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n7 = (*hxc_l_reference).hxc_id;
    if (hxc_l_tmp_class_field_load_result_n6.data == NULL || hxc_l_tmp_class_field_load_result_n7.data == NULL ? hxc_l_tmp_class_field_load_result_n6.data == hxc_l_tmp_class_field_load_result_n7.data : hxc_l_tmp_class_field_load_result_n6.byte_length == hxc_l_tmp_class_field_load_result_n7.byte_length && (hxc_l_tmp_class_field_load_result_n6.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n6.data, hxc_l_tmp_class_field_load_result_n7.data, hxc_l_tmp_class_field_load_result_n6.byte_length) == 0))
    {
      if (hxc_l_reader == NULL)
      {
        abort();
      }
      int32_t hxc_l_tmp_class_field_load_result_n9 = (*hxc_l_reference).hxc_line;
      int32_t hxc_l_tmp_class_field_load_result_n10 = (*hxc_l_reference).hxc_column;
      hxc_l_tmp_enum_payload_0_owner_n7 = hxc_l_path;
      if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n7) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_string hxc_l_tmp_enum_payload_0_owned_load_result_n11 = hxc_l_tmp_enum_payload_0_owner_n7;
      hxc_string hxc_l_tmp_class_field_load_result_n12 = (*hxc_l_reference).hxc_id;
      hxc_l_tmp_enum_payload_1_owner_n8 = hxc_l_tmp_class_field_load_result_n12;
      if (hxc_string_retain(hxc_l_tmp_enum_payload_1_owner_n8) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_string hxc_l_tmp_enum_payload_1_owned_load_result_n13 = hxc_l_tmp_enum_payload_1_owner_n8;
      struct hxc_caxecraft_content_RuntimeContentPack_RuntimeKindId *hxc_l_tmp_load_result_n14 = hxc_l_entry;
      hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n14;
      if (hxc_l_tmp_load_result_n14 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_class_field_load_result_n15 = (*hxc_l_tmp_load_result_n14).hxc_kind;
      hxc_l_tmp_enum_payload_2_owner_n9 = hxc_l_tmp_class_field_load_result_n15;
      if (hxc_string_retain(hxc_l_tmp_enum_payload_2_owner_n9) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_instance_call_argument_2_owner_n10 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaWrongReferenceKind, .hxc_payload.hxc_SchemaWrongReferenceKind.hxc_path = hxc_l_tmp_enum_payload_0_owned_load_result_n11, .hxc_payload.hxc_SchemaWrongReferenceKind.hxc_id = hxc_l_tmp_enum_payload_1_owned_load_result_n13, .hxc_payload.hxc_SchemaWrongReferenceKind.hxc_actualKind = hxc_l_tmp_enum_payload_2_owner_n9 };
      hxc_caxecraft_content_RuntimeSchemaReader_rejectAt(hxc_l_reader, hxc_l_tmp_class_field_load_result_n9, hxc_l_tmp_class_field_load_result_n10, hxc_l_tmp_instance_call_argument_2_owner_n10);
      hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_2_owner_n10);
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return false;
    }
  }
  if (hxc_l_reader == NULL)
  {
    abort();
  }
  if (hxc_l_reference == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n19 = (*hxc_l_reference).hxc_line;
  int32_t hxc_l_tmp_class_field_load_result_n20 = (*hxc_l_reference).hxc_column;
  hxc_string hxc_l_tmp_enum_payload_0_owner_n11 = hxc_l_path;
  if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n11) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_enum_payload_0_owned_load_result_n21 = hxc_l_tmp_enum_payload_0_owner_n11;
  hxc_string hxc_l_tmp_class_field_load_result_n22 = (*hxc_l_reference).hxc_id;
  hxc_string hxc_l_tmp_enum_payload_1_owner_n12 = hxc_l_tmp_class_field_load_result_n22;
  if (hxc_string_retain(hxc_l_tmp_enum_payload_1_owner_n12) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_enum_payload_1_owned_load_result_n23 = hxc_l_tmp_enum_payload_1_owner_n12;
  hxc_string hxc_l_tmp_enum_payload_2_owner_n13 = hxc_l_expectedKind;
  if (hxc_string_retain(hxc_l_tmp_enum_payload_2_owner_n13) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_instance_call_argument_2_owner_n14 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaUnresolvedReference, .hxc_payload.hxc_SchemaUnresolvedReference.hxc_path = hxc_l_tmp_enum_payload_0_owned_load_result_n21, .hxc_payload.hxc_SchemaUnresolvedReference.hxc_id = hxc_l_tmp_enum_payload_1_owned_load_result_n23, .hxc_payload.hxc_SchemaUnresolvedReference.hxc_expectedKind = hxc_l_tmp_enum_payload_2_owner_n13 };
  hxc_caxecraft_content_RuntimeSchemaReader_rejectAt(hxc_l_reader, hxc_l_tmp_class_field_load_result_n19, hxc_l_tmp_class_field_load_result_n20, hxc_l_tmp_instance_call_argument_2_owner_n14);
  hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_2_owner_n14);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return false;
}

struct hxc_caxecraft_content_RuntimeContentPackResult hxc_caxecraft_content_RuntimeContentPack_rejected(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader)
{
  const void *hxc_l_gc_roots[2] = { NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 2, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeSchemaDiagnostic hxc_l_tmp_enum_payload_0_owner_n3 = { 0 };
  if (hxc_l_reader == NULL)
  {
    abort();
  }
  struct hxc_optional_caxecraft_content_RuntimeSchemaDiagnostic hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_reader).hxc_failure;
  struct hxc_optional_caxecraft_content_RuntimeSchemaDiagnostic hxc_l_diagnostic = hxc_l_tmp_class_field_load_result_n0;
  if (hxc_optional_a967a598_retain(&hxc_l_diagnostic) != HXC_STATUS_OK)
  {
    abort();
  }
  if (!!hxc_l_diagnostic.hxc_has_value)
  {
    struct hxc_optional_caxecraft_content_RuntimeSchemaDiagnostic hxc_l_tmp_load_result_n6 = hxc_l_diagnostic;
    if (!hxc_l_tmp_load_result_n6.hxc_has_value)
    {
      abort();
    }
    hxc_l_tmp_enum_payload_0_owner_n3 = hxc_l_tmp_load_result_n6.hxc_value;
    if (hxc_record_c8d3a25f_retain(&hxc_l_tmp_enum_payload_0_owner_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimeSchemaDiagnostic hxc_l_tmp_enum_payload_0_owned_load_result_n8 = hxc_l_tmp_enum_payload_0_owner_n3;
    hxc_l_gc_roots[1] = (struct hxc_caxecraft_content_RuntimeContentPackResult){ .hxc_tag = hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackRejected, .hxc_payload.hxc_RuntimeContentPackRejected.hxc_diagnostic = hxc_l_tmp_enum_payload_0_owned_load_result_n8 }.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)(struct hxc_caxecraft_content_RuntimeContentPackResult){ .hxc_tag = hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackRejected, .hxc_payload.hxc_RuntimeContentPackRejected.hxc_diagnostic = hxc_l_tmp_enum_payload_0_owned_load_result_n8 }.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
    hxc_optional_a967a598_destroy(&hxc_l_diagnostic);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_RuntimeContentPackResult){ .hxc_tag = hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackRejected, .hxc_payload.hxc_RuntimeContentPackRejected.hxc_diagnostic = hxc_l_tmp_enum_payload_0_owned_load_result_n8 };
  }
  hxc_string hxc_l_tmp_enum_payload_0_owner_n2 = (hxc_string){ (const uint8_t *)"decoder", 7, true, NULL };
  if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_enum_payload_0_owned_load_result_n2 = hxc_l_tmp_enum_payload_0_owner_n2;
  hxc_l_gc_roots[0] = (struct hxc_caxecraft_content_RuntimeContentPackResult){ .hxc_tag = hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackRejected, .hxc_payload.hxc_RuntimeContentPackRejected.hxc_diagnostic = (struct hxc_caxecraft_content_RuntimeSchemaDiagnostic){ .hxc_column = 1, .hxc_kind = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidInvariant, .hxc_payload.hxc_SchemaInvalidInvariant.hxc_path = hxc_l_tmp_enum_payload_0_owned_load_result_n2 }, .hxc_line = 1 } }.hxc_tag == hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackReady ? (const void *)(struct hxc_caxecraft_content_RuntimeContentPackResult){ .hxc_tag = hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackRejected, .hxc_payload.hxc_RuntimeContentPackRejected.hxc_diagnostic = (struct hxc_caxecraft_content_RuntimeSchemaDiagnostic){ .hxc_column = 1, .hxc_kind = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidInvariant, .hxc_payload.hxc_SchemaInvalidInvariant.hxc_path = hxc_l_tmp_enum_payload_0_owned_load_result_n2 }, .hxc_line = 1 } }.hxc_payload.hxc_RuntimeContentPackReady.hxc_registry : NULL;
  hxc_optional_a967a598_destroy(&hxc_l_diagnostic);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_content_RuntimeContentPackResult){ .hxc_tag = hxc_caxecraft_content_RuntimeContentPackResult_RuntimeContentPackRejected, .hxc_payload.hxc_RuntimeContentPackRejected.hxc_diagnostic = (struct hxc_caxecraft_content_RuntimeSchemaDiagnostic){ .hxc_column = 1, .hxc_kind = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidInvariant, .hxc_payload.hxc_SchemaInvalidInvariant.hxc_path = hxc_l_tmp_enum_payload_0_owned_load_result_n2 }, .hxc_line = 1 } };
}

bool hxc_caxecraft_content_RuntimeContentPack_validateAquaticOrder(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, hxc_string hxc_l_path, struct hxc_array_ref *hxc_l_values)
{
  const void *hxc_l_gc_roots[5] = { (const void *)hxc_l_values, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 5, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g = 1;
  int32_t hxc_l_tmp_array_length_result_n0;
  if (hxc_array_ref_length(hxc_l_values, &hxc_l_tmp_array_length_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_g;
    if (!(hxc_l_tmp_load_result_n1 < hxc_l_g1))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n3 = hxc_l_g;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n3, 1);
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n3;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition *hxc_l_tmp_array_get_result_n5;
    if (hxc_array_ref_get_copy(hxc_l_values, (size_t)hxc_i32_subtract_wrapping(hxc_l_index, 1), &hxc_l_tmp_array_get_result_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_array_get_result_n5;
    hxc_l_gc_roots[2] = (const void *)(hxc_l_tmp_array_get_result_n5 == NULL ? NULL : &hxc_l_tmp_array_get_result_n5->hxc_base);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition *hxc_l_tmp_array_get_result_n7;
    if (hxc_array_ref_get_copy(hxc_l_values, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_array_get_result_n7;
    hxc_l_gc_roots[4] = (const void *)(hxc_l_tmp_array_get_result_n7 == NULL ? NULL : &hxc_l_tmp_array_get_result_n7->hxc_base);
    bool hxc_l_tmp_call_result_n8 = hxc_caxecraft_content_RuntimeContentPack_validatePair(hxc_l_reader, hxc_l_path, hxc_l_tmp_array_get_result_n5 == NULL ? NULL : &hxc_l_tmp_array_get_result_n5->hxc_base, hxc_l_tmp_array_get_result_n7 == NULL ? NULL : &hxc_l_tmp_array_get_result_n7->hxc_base);
    if (!hxc_l_tmp_call_result_n8)
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

bool hxc_caxecraft_content_RuntimeContentPack_validateBlockOrder(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, hxc_string hxc_l_path, struct hxc_array_ref *hxc_l_values)
{
  const void *hxc_l_gc_roots[5] = { (const void *)hxc_l_values, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 5, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g = 1;
  int32_t hxc_l_tmp_array_length_result_n0;
  if (hxc_array_ref_length(hxc_l_values, &hxc_l_tmp_array_length_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_g;
    if (!(hxc_l_tmp_load_result_n1 < hxc_l_g1))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n3 = hxc_l_g;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n3, 1);
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n3;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_array_get_result_n5;
    if (hxc_array_ref_get_copy(hxc_l_values, (size_t)hxc_i32_subtract_wrapping(hxc_l_index, 1), &hxc_l_tmp_array_get_result_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_array_get_result_n5;
    hxc_l_gc_roots[2] = (const void *)(hxc_l_tmp_array_get_result_n5 == NULL ? NULL : &hxc_l_tmp_array_get_result_n5->hxc_base);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_array_get_result_n7;
    if (hxc_array_ref_get_copy(hxc_l_values, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_array_get_result_n7;
    hxc_l_gc_roots[4] = (const void *)(hxc_l_tmp_array_get_result_n7 == NULL ? NULL : &hxc_l_tmp_array_get_result_n7->hxc_base);
    bool hxc_l_tmp_call_result_n8 = hxc_caxecraft_content_RuntimeContentPack_validatePair(hxc_l_reader, hxc_l_path, hxc_l_tmp_array_get_result_n5 == NULL ? NULL : &hxc_l_tmp_array_get_result_n5->hxc_base, hxc_l_tmp_array_get_result_n7 == NULL ? NULL : &hxc_l_tmp_array_get_result_n7->hxc_base);
    if (!hxc_l_tmp_call_result_n8)
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

bool hxc_caxecraft_content_RuntimeContentPack_validateDropOrder(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, hxc_string hxc_l_path, struct hxc_array_ref *hxc_l_values)
{
  const void *hxc_l_gc_roots[5] = { (const void *)hxc_l_values, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 5, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g = 1;
  int32_t hxc_l_tmp_array_length_result_n0;
  if (hxc_array_ref_length(hxc_l_values, &hxc_l_tmp_array_length_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_g;
    if (!(hxc_l_tmp_load_result_n1 < hxc_l_g1))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n3 = hxc_l_g;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n3, 1);
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n3;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeDropDefinition *hxc_l_tmp_array_get_result_n5;
    if (hxc_array_ref_get_copy(hxc_l_values, (size_t)hxc_i32_subtract_wrapping(hxc_l_index, 1), &hxc_l_tmp_array_get_result_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_array_get_result_n5;
    hxc_l_gc_roots[2] = (const void *)(hxc_l_tmp_array_get_result_n5 == NULL ? NULL : &hxc_l_tmp_array_get_result_n5->hxc_base);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeDropDefinition *hxc_l_tmp_array_get_result_n7;
    if (hxc_array_ref_get_copy(hxc_l_values, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_array_get_result_n7;
    hxc_l_gc_roots[4] = (const void *)(hxc_l_tmp_array_get_result_n7 == NULL ? NULL : &hxc_l_tmp_array_get_result_n7->hxc_base);
    bool hxc_l_tmp_call_result_n8 = hxc_caxecraft_content_RuntimeContentPack_validatePair(hxc_l_reader, hxc_l_path, hxc_l_tmp_array_get_result_n5 == NULL ? NULL : &hxc_l_tmp_array_get_result_n5->hxc_base, hxc_l_tmp_array_get_result_n7 == NULL ? NULL : &hxc_l_tmp_array_get_result_n7->hxc_base);
    if (!hxc_l_tmp_call_result_n8)
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

bool hxc_caxecraft_content_RuntimeContentPack_validateEffectOrder(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, hxc_string hxc_l_path, struct hxc_array_ref *hxc_l_values)
{
  const void *hxc_l_gc_roots[5] = { (const void *)hxc_l_values, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 5, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g = 1;
  int32_t hxc_l_tmp_array_length_result_n0;
  if (hxc_array_ref_length(hxc_l_values, &hxc_l_tmp_array_length_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_g;
    if (!(hxc_l_tmp_load_result_n1 < hxc_l_g1))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n3 = hxc_l_g;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n3, 1);
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n3;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEffectDefinition *hxc_l_tmp_array_get_result_n5;
    if (hxc_array_ref_get_copy(hxc_l_values, (size_t)hxc_i32_subtract_wrapping(hxc_l_index, 1), &hxc_l_tmp_array_get_result_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_array_get_result_n5;
    hxc_l_gc_roots[2] = (const void *)(hxc_l_tmp_array_get_result_n5 == NULL ? NULL : &hxc_l_tmp_array_get_result_n5->hxc_base);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEffectDefinition *hxc_l_tmp_array_get_result_n7;
    if (hxc_array_ref_get_copy(hxc_l_values, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_array_get_result_n7;
    hxc_l_gc_roots[4] = (const void *)(hxc_l_tmp_array_get_result_n7 == NULL ? NULL : &hxc_l_tmp_array_get_result_n7->hxc_base);
    bool hxc_l_tmp_call_result_n8 = hxc_caxecraft_content_RuntimeContentPack_validatePair(hxc_l_reader, hxc_l_path, hxc_l_tmp_array_get_result_n5 == NULL ? NULL : &hxc_l_tmp_array_get_result_n5->hxc_base, hxc_l_tmp_array_get_result_n7 == NULL ? NULL : &hxc_l_tmp_array_get_result_n7->hxc_base);
    if (!hxc_l_tmp_call_result_n8)
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

bool hxc_caxecraft_content_RuntimeContentPack_validateEnemyOrder(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, hxc_string hxc_l_path, struct hxc_array_ref *hxc_l_values)
{
  const void *hxc_l_gc_roots[5] = { (const void *)hxc_l_values, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 5, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g = 1;
  int32_t hxc_l_tmp_array_length_result_n0;
  if (hxc_array_ref_length(hxc_l_values, &hxc_l_tmp_array_length_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_g;
    if (!(hxc_l_tmp_load_result_n1 < hxc_l_g1))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n3 = hxc_l_g;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n3, 1);
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n3;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_tmp_array_get_result_n5;
    if (hxc_array_ref_get_copy(hxc_l_values, (size_t)hxc_i32_subtract_wrapping(hxc_l_index, 1), &hxc_l_tmp_array_get_result_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_array_get_result_n5;
    hxc_l_gc_roots[2] = (const void *)(hxc_l_tmp_array_get_result_n5 == NULL ? NULL : &hxc_l_tmp_array_get_result_n5->hxc_base);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_tmp_array_get_result_n7;
    if (hxc_array_ref_get_copy(hxc_l_values, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_array_get_result_n7;
    hxc_l_gc_roots[4] = (const void *)(hxc_l_tmp_array_get_result_n7 == NULL ? NULL : &hxc_l_tmp_array_get_result_n7->hxc_base);
    bool hxc_l_tmp_call_result_n8 = hxc_caxecraft_content_RuntimeContentPack_validatePair(hxc_l_reader, hxc_l_path, hxc_l_tmp_array_get_result_n5 == NULL ? NULL : &hxc_l_tmp_array_get_result_n5->hxc_base, hxc_l_tmp_array_get_result_n7 == NULL ? NULL : &hxc_l_tmp_array_get_result_n7->hxc_base);
    if (!hxc_l_tmp_call_result_n8)
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

bool hxc_caxecraft_content_RuntimeContentPack_validateFluidOrder(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, hxc_string hxc_l_path, struct hxc_array_ref *hxc_l_values)
{
  const void *hxc_l_gc_roots[5] = { (const void *)hxc_l_values, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 5, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g = 1;
  int32_t hxc_l_tmp_array_length_result_n0;
  if (hxc_array_ref_length(hxc_l_values, &hxc_l_tmp_array_length_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_g;
    if (!(hxc_l_tmp_load_result_n1 < hxc_l_g1))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n3 = hxc_l_g;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n3, 1);
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n3;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeFluidDefinition *hxc_l_tmp_array_get_result_n5;
    if (hxc_array_ref_get_copy(hxc_l_values, (size_t)hxc_i32_subtract_wrapping(hxc_l_index, 1), &hxc_l_tmp_array_get_result_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_array_get_result_n5;
    hxc_l_gc_roots[2] = (const void *)(hxc_l_tmp_array_get_result_n5 == NULL ? NULL : &hxc_l_tmp_array_get_result_n5->hxc_base);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeFluidDefinition *hxc_l_tmp_array_get_result_n7;
    if (hxc_array_ref_get_copy(hxc_l_values, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_array_get_result_n7;
    hxc_l_gc_roots[4] = (const void *)(hxc_l_tmp_array_get_result_n7 == NULL ? NULL : &hxc_l_tmp_array_get_result_n7->hxc_base);
    bool hxc_l_tmp_call_result_n8 = hxc_caxecraft_content_RuntimeContentPack_validatePair(hxc_l_reader, hxc_l_path, hxc_l_tmp_array_get_result_n5 == NULL ? NULL : &hxc_l_tmp_array_get_result_n5->hxc_base, hxc_l_tmp_array_get_result_n7 == NULL ? NULL : &hxc_l_tmp_array_get_result_n7->hxc_base);
    if (!hxc_l_tmp_call_result_n8)
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

bool hxc_caxecraft_content_RuntimeContentPack_validateItemOrder(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, hxc_string hxc_l_path, struct hxc_array_ref *hxc_l_values)
{
  const void *hxc_l_gc_roots[5] = { (const void *)hxc_l_values, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 5, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g = 1;
  int32_t hxc_l_tmp_array_length_result_n0;
  if (hxc_array_ref_length(hxc_l_values, &hxc_l_tmp_array_length_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_g;
    if (!(hxc_l_tmp_load_result_n1 < hxc_l_g1))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n3 = hxc_l_g;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n3, 1);
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n3;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_tmp_array_get_result_n5;
    if (hxc_array_ref_get_copy(hxc_l_values, (size_t)hxc_i32_subtract_wrapping(hxc_l_index, 1), &hxc_l_tmp_array_get_result_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_array_get_result_n5;
    hxc_l_gc_roots[2] = (const void *)(hxc_l_tmp_array_get_result_n5 == NULL ? NULL : &hxc_l_tmp_array_get_result_n5->hxc_base);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_tmp_array_get_result_n7;
    if (hxc_array_ref_get_copy(hxc_l_values, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_array_get_result_n7;
    hxc_l_gc_roots[4] = (const void *)(hxc_l_tmp_array_get_result_n7 == NULL ? NULL : &hxc_l_tmp_array_get_result_n7->hxc_base);
    bool hxc_l_tmp_call_result_n8 = hxc_caxecraft_content_RuntimeContentPack_validatePair(hxc_l_reader, hxc_l_path, hxc_l_tmp_array_get_result_n5 == NULL ? NULL : &hxc_l_tmp_array_get_result_n5->hxc_base, hxc_l_tmp_array_get_result_n7 == NULL ? NULL : &hxc_l_tmp_array_get_result_n7->hxc_base);
    if (!hxc_l_tmp_call_result_n8)
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

bool hxc_caxecraft_content_RuntimeContentPack_validateNpcOrder(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, hxc_string hxc_l_path, struct hxc_array_ref *hxc_l_values)
{
  const void *hxc_l_gc_roots[5] = { (const void *)hxc_l_values, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 5, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g = 1;
  int32_t hxc_l_tmp_array_length_result_n0;
  if (hxc_array_ref_length(hxc_l_values, &hxc_l_tmp_array_length_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_g;
    if (!(hxc_l_tmp_load_result_n1 < hxc_l_g1))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n3 = hxc_l_g;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n3, 1);
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n3;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeNpcDefinition *hxc_l_tmp_array_get_result_n5;
    if (hxc_array_ref_get_copy(hxc_l_values, (size_t)hxc_i32_subtract_wrapping(hxc_l_index, 1), &hxc_l_tmp_array_get_result_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_array_get_result_n5;
    hxc_l_gc_roots[2] = (const void *)(hxc_l_tmp_array_get_result_n5 == NULL ? NULL : &hxc_l_tmp_array_get_result_n5->hxc_base);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeNpcDefinition *hxc_l_tmp_array_get_result_n7;
    if (hxc_array_ref_get_copy(hxc_l_values, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_array_get_result_n7;
    hxc_l_gc_roots[4] = (const void *)(hxc_l_tmp_array_get_result_n7 == NULL ? NULL : &hxc_l_tmp_array_get_result_n7->hxc_base);
    bool hxc_l_tmp_call_result_n8 = hxc_caxecraft_content_RuntimeContentPack_validatePair(hxc_l_reader, hxc_l_path, hxc_l_tmp_array_get_result_n5 == NULL ? NULL : &hxc_l_tmp_array_get_result_n5->hxc_base, hxc_l_tmp_array_get_result_n7 == NULL ? NULL : &hxc_l_tmp_array_get_result_n7->hxc_base);
    if (!hxc_l_tmp_call_result_n8)
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

bool hxc_caxecraft_content_RuntimeContentPack_validateOrder(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, struct hxc_caxecraft_content_ContentJsonNode *hxc_l_node, hxc_string hxc_l_path, struct hxc_array_ref *hxc_l_values)
{
  const void *hxc_l_gc_roots[4] = { (const void *)hxc_l_node, (const void *)hxc_l_values, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 4, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g = 1;
  int32_t hxc_l_tmp_array_length_result_n0;
  if (hxc_array_ref_length(hxc_l_values, &hxc_l_tmp_array_length_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_g;
    if (!(hxc_l_tmp_load_result_n1 < hxc_l_g1))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n3 = hxc_l_g;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n3, 1);
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n3;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeLocatedId *hxc_l_tmp_array_get_result_n5;
    if (hxc_array_ref_get_copy(hxc_l_values, (size_t)hxc_i32_subtract_wrapping(hxc_l_index, 1), &hxc_l_tmp_array_get_result_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_array_get_result_n5;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeLocatedId *hxc_l_tmp_array_get_result_n7;
    if (hxc_array_ref_get_copy(hxc_l_values, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_array_get_result_n7;
    bool hxc_l_tmp_call_result_n8 = hxc_caxecraft_content_RuntimeContentPack_validatePair(hxc_l_reader, hxc_l_path, hxc_l_tmp_array_get_result_n5, hxc_l_tmp_array_get_result_n7);
    if (!hxc_l_tmp_call_result_n8)
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

bool hxc_caxecraft_content_RuntimeContentPack_validatePack(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_airReference, struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_defaultAquaticReference, struct hxc_array_ref *hxc_l_features, struct hxc_array_ref *hxc_l_blocks, struct hxc_array_ref *hxc_l_fluids, struct hxc_array_ref *hxc_l_aquatic, struct hxc_array_ref *hxc_l_items, struct hxc_array_ref *hxc_l_npcs, struct hxc_array_ref *hxc_l_enemies, struct hxc_array_ref *hxc_l_drops, struct hxc_array_ref *hxc_l_effects)
{
  const void *hxc_l_gc_roots[118] = { (const void *)hxc_l_airReference, (const void *)hxc_l_defaultAquaticReference, (const void *)hxc_l_features, (const void *)hxc_l_blocks, (const void *)hxc_l_fluids, (const void *)hxc_l_aquatic, (const void *)hxc_l_items, (const void *)hxc_l_npcs, (const void *)hxc_l_enemies, (const void *)hxc_l_drops, (const void *)hxc_l_effects, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 118, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_2_owner_n38 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n39 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_1_owner_n40 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_2_owner_n41 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_2_owner_n42 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n46 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_2_owner_n47 = { 0 };
  hxc_string hxc_l_tmp_string_equality_left_owner_n48 = { 0 };
  hxc_string hxc_l_tmp_string_equality_left_owner_n53 = { 0 };
  hxc_string hxc_l_tmp_string_equality_left_owner_n59 = { 0 };
  hxc_string hxc_l_tmp_string_equality_left_owner_n63 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n68 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_2_owner_n69 = { 0 };
  hxc_string hxc_l_tmp_string_equality_left_owner_n72 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n0 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_array_b828e297_descriptor, (void **)&hxc_l_tmp_array_create_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_init_in_place(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeKindId *), _Alignof(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeKindId *), NULL, NULL, NULL, NULL }, hxc_l_tmp_array_create_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[11] = (const void *)hxc_l_tmp_array_create_result_n0;
  struct hxc_array_ref *hxc_l_kinds = hxc_l_tmp_array_create_result_n0;
  int32_t hxc_l_g_hb469d0a44578 = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_g_hb469d0a44578;
    int32_t hxc_l_tmp_array_length_result_n2;
    if (hxc_array_ref_length(hxc_l_features, &hxc_l_tmp_array_length_result_n2) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n1 < hxc_l_tmp_array_length_result_n2))
    {
      break;
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeLocatedId *hxc_l_tmp_array_get_result_n4;
    if (hxc_array_ref_get_copy(hxc_l_features, (size_t)hxc_l_g_hb469d0a44578, &hxc_l_tmp_array_get_result_n4) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[12] = (const void *)hxc_l_tmp_array_get_result_n4;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeLocatedId *hxc_l_entry_h309b92e58bc4 = hxc_l_tmp_array_get_result_n4;
    hxc_l_g_hb469d0a44578 = hxc_i32_add_wrapping(hxc_l_g_hb469d0a44578, 1);
    struct hxc_array_ref *hxc_l_tmp_load_result_n6 = hxc_l_kinds;
    hxc_l_gc_roots[13] = (const void *)hxc_l_tmp_load_result_n6;
    hxc_l_gc_roots[14] = (const void *)hxc_l_entry_h309b92e58bc4;
    bool hxc_l_tmp_call_result_n8 = hxc_caxecraft_content_RuntimeContentPack_addKind(hxc_l_reader, hxc_l_tmp_load_result_n6, hxc_l_entry_h309b92e58bc4, (hxc_string){ (const uint8_t *)"feature", 7, true, NULL });
    if (!hxc_l_tmp_call_result_n8)
    {
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return false;
    }
  }
  int32_t hxc_l_g_h5d51e7c212b7 = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n9 = hxc_l_g_h5d51e7c212b7;
    int32_t hxc_l_tmp_array_length_result_n10;
    if (hxc_array_ref_length(hxc_l_blocks, &hxc_l_tmp_array_length_result_n10) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n9 < hxc_l_tmp_array_length_result_n10))
    {
      break;
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_array_get_result_n12;
    if (hxc_array_ref_get_copy(hxc_l_blocks, (size_t)hxc_l_g_h5d51e7c212b7, &hxc_l_tmp_array_get_result_n12) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[15] = (const void *)hxc_l_tmp_array_get_result_n12;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_entry_h1b5318bfe743 = hxc_l_tmp_array_get_result_n12;
    hxc_l_g_h5d51e7c212b7 = hxc_i32_add_wrapping(hxc_l_g_h5d51e7c212b7, 1);
    struct hxc_array_ref *hxc_l_tmp_load_result_n14 = hxc_l_kinds;
    hxc_l_gc_roots[16] = (const void *)hxc_l_tmp_load_result_n14;
    hxc_l_gc_roots[17] = (const void *)hxc_l_entry_h1b5318bfe743;
    hxc_l_gc_roots[18] = (const void *)(hxc_l_entry_h1b5318bfe743 == NULL ? NULL : &hxc_l_entry_h1b5318bfe743->hxc_base);
    bool hxc_l_tmp_call_result_n16 = hxc_caxecraft_content_RuntimeContentPack_addKind(hxc_l_reader, hxc_l_tmp_load_result_n14, hxc_l_entry_h1b5318bfe743 == NULL ? NULL : &hxc_l_entry_h1b5318bfe743->hxc_base, (hxc_string){ (const uint8_t *)"block", 5, true, NULL });
    if (!hxc_l_tmp_call_result_n16)
    {
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return false;
    }
  }
  int32_t hxc_l_g_h40180b7a7cc4 = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n17 = hxc_l_g_h40180b7a7cc4;
    int32_t hxc_l_tmp_array_length_result_n18;
    if (hxc_array_ref_length(hxc_l_fluids, &hxc_l_tmp_array_length_result_n18) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n17 < hxc_l_tmp_array_length_result_n18))
    {
      break;
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeFluidDefinition *hxc_l_tmp_array_get_result_n20;
    if (hxc_array_ref_get_copy(hxc_l_fluids, (size_t)hxc_l_g_h40180b7a7cc4, &hxc_l_tmp_array_get_result_n20) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[19] = (const void *)hxc_l_tmp_array_get_result_n20;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeFluidDefinition *hxc_l_entry_heeb11d3c0339 = hxc_l_tmp_array_get_result_n20;
    hxc_l_g_h40180b7a7cc4 = hxc_i32_add_wrapping(hxc_l_g_h40180b7a7cc4, 1);
    struct hxc_array_ref *hxc_l_tmp_load_result_n22 = hxc_l_kinds;
    hxc_l_gc_roots[20] = (const void *)hxc_l_tmp_load_result_n22;
    hxc_l_gc_roots[21] = (const void *)hxc_l_entry_heeb11d3c0339;
    hxc_l_gc_roots[22] = (const void *)(hxc_l_entry_heeb11d3c0339 == NULL ? NULL : &hxc_l_entry_heeb11d3c0339->hxc_base);
    bool hxc_l_tmp_call_result_n24 = hxc_caxecraft_content_RuntimeContentPack_addKind(hxc_l_reader, hxc_l_tmp_load_result_n22, hxc_l_entry_heeb11d3c0339 == NULL ? NULL : &hxc_l_entry_heeb11d3c0339->hxc_base, (hxc_string){ (const uint8_t *)"fluid", 5, true, NULL });
    if (!hxc_l_tmp_call_result_n24)
    {
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return false;
    }
  }
  int32_t hxc_l_g_h68f7e493267d = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n25 = hxc_l_g_h68f7e493267d;
    int32_t hxc_l_tmp_array_length_result_n26;
    if (hxc_array_ref_length(hxc_l_aquatic, &hxc_l_tmp_array_length_result_n26) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n25 < hxc_l_tmp_array_length_result_n26))
    {
      break;
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition *hxc_l_tmp_array_get_result_n28;
    if (hxc_array_ref_get_copy(hxc_l_aquatic, (size_t)hxc_l_g_h68f7e493267d, &hxc_l_tmp_array_get_result_n28) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[23] = (const void *)hxc_l_tmp_array_get_result_n28;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition *hxc_l_entry_h3294843d17e6 = hxc_l_tmp_array_get_result_n28;
    hxc_l_g_h68f7e493267d = hxc_i32_add_wrapping(hxc_l_g_h68f7e493267d, 1);
    struct hxc_array_ref *hxc_l_tmp_load_result_n30 = hxc_l_kinds;
    hxc_l_gc_roots[24] = (const void *)hxc_l_tmp_load_result_n30;
    hxc_l_gc_roots[25] = (const void *)hxc_l_entry_h3294843d17e6;
    hxc_l_gc_roots[26] = (const void *)(hxc_l_entry_h3294843d17e6 == NULL ? NULL : &hxc_l_entry_h3294843d17e6->hxc_base);
    bool hxc_l_tmp_call_result_n32 = hxc_caxecraft_content_RuntimeContentPack_addKind(hxc_l_reader, hxc_l_tmp_load_result_n30, hxc_l_entry_h3294843d17e6 == NULL ? NULL : &hxc_l_entry_h3294843d17e6->hxc_base, (hxc_string){ (const uint8_t *)"aquatic profile", 15, true, NULL });
    if (!hxc_l_tmp_call_result_n32)
    {
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return false;
    }
  }
  int32_t hxc_l_g_hbea2ac4c14d1 = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n33 = hxc_l_g_hbea2ac4c14d1;
    int32_t hxc_l_tmp_array_length_result_n34;
    if (hxc_array_ref_length(hxc_l_items, &hxc_l_tmp_array_length_result_n34) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n33 < hxc_l_tmp_array_length_result_n34))
    {
      break;
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_tmp_array_get_result_n36;
    if (hxc_array_ref_get_copy(hxc_l_items, (size_t)hxc_l_g_hbea2ac4c14d1, &hxc_l_tmp_array_get_result_n36) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[27] = (const void *)hxc_l_tmp_array_get_result_n36;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_entry_h61378bb0ba99 = hxc_l_tmp_array_get_result_n36;
    hxc_l_g_hbea2ac4c14d1 = hxc_i32_add_wrapping(hxc_l_g_hbea2ac4c14d1, 1);
    struct hxc_array_ref *hxc_l_tmp_load_result_n38 = hxc_l_kinds;
    hxc_l_gc_roots[28] = (const void *)hxc_l_tmp_load_result_n38;
    hxc_l_gc_roots[29] = (const void *)hxc_l_entry_h61378bb0ba99;
    hxc_l_gc_roots[30] = (const void *)(hxc_l_entry_h61378bb0ba99 == NULL ? NULL : &hxc_l_entry_h61378bb0ba99->hxc_base);
    bool hxc_l_tmp_call_result_n40 = hxc_caxecraft_content_RuntimeContentPack_addKind(hxc_l_reader, hxc_l_tmp_load_result_n38, hxc_l_entry_h61378bb0ba99 == NULL ? NULL : &hxc_l_entry_h61378bb0ba99->hxc_base, (hxc_string){ (const uint8_t *)"item", 4, true, NULL });
    if (!hxc_l_tmp_call_result_n40)
    {
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return false;
    }
  }
  int32_t hxc_l_g_h2aeb2629b04c = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n41 = hxc_l_g_h2aeb2629b04c;
    int32_t hxc_l_tmp_array_length_result_n42;
    if (hxc_array_ref_length(hxc_l_npcs, &hxc_l_tmp_array_length_result_n42) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n41 < hxc_l_tmp_array_length_result_n42))
    {
      break;
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeNpcDefinition *hxc_l_tmp_array_get_result_n44;
    if (hxc_array_ref_get_copy(hxc_l_npcs, (size_t)hxc_l_g_h2aeb2629b04c, &hxc_l_tmp_array_get_result_n44) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[31] = (const void *)hxc_l_tmp_array_get_result_n44;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeNpcDefinition *hxc_l_entry_h3b6060e3728b = hxc_l_tmp_array_get_result_n44;
    hxc_l_g_h2aeb2629b04c = hxc_i32_add_wrapping(hxc_l_g_h2aeb2629b04c, 1);
    struct hxc_array_ref *hxc_l_tmp_load_result_n46 = hxc_l_kinds;
    hxc_l_gc_roots[32] = (const void *)hxc_l_tmp_load_result_n46;
    hxc_l_gc_roots[33] = (const void *)hxc_l_entry_h3b6060e3728b;
    hxc_l_gc_roots[34] = (const void *)(hxc_l_entry_h3b6060e3728b == NULL ? NULL : &hxc_l_entry_h3b6060e3728b->hxc_base);
    bool hxc_l_tmp_call_result_n48 = hxc_caxecraft_content_RuntimeContentPack_addKind(hxc_l_reader, hxc_l_tmp_load_result_n46, hxc_l_entry_h3b6060e3728b == NULL ? NULL : &hxc_l_entry_h3b6060e3728b->hxc_base, (hxc_string){ (const uint8_t *)"npc", 3, true, NULL });
    if (!hxc_l_tmp_call_result_n48)
    {
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return false;
    }
  }
  int32_t hxc_l_g_h75f71c794b83 = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n49 = hxc_l_g_h75f71c794b83;
    int32_t hxc_l_tmp_array_length_result_n50;
    if (hxc_array_ref_length(hxc_l_enemies, &hxc_l_tmp_array_length_result_n50) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n49 < hxc_l_tmp_array_length_result_n50))
    {
      break;
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_tmp_array_get_result_n52;
    if (hxc_array_ref_get_copy(hxc_l_enemies, (size_t)hxc_l_g_h75f71c794b83, &hxc_l_tmp_array_get_result_n52) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[35] = (const void *)hxc_l_tmp_array_get_result_n52;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_entry_hc15484524557 = hxc_l_tmp_array_get_result_n52;
    hxc_l_g_h75f71c794b83 = hxc_i32_add_wrapping(hxc_l_g_h75f71c794b83, 1);
    struct hxc_array_ref *hxc_l_tmp_load_result_n54 = hxc_l_kinds;
    hxc_l_gc_roots[36] = (const void *)hxc_l_tmp_load_result_n54;
    hxc_l_gc_roots[37] = (const void *)hxc_l_entry_hc15484524557;
    hxc_l_gc_roots[38] = (const void *)(hxc_l_entry_hc15484524557 == NULL ? NULL : &hxc_l_entry_hc15484524557->hxc_base);
    bool hxc_l_tmp_call_result_n56 = hxc_caxecraft_content_RuntimeContentPack_addKind(hxc_l_reader, hxc_l_tmp_load_result_n54, hxc_l_entry_hc15484524557 == NULL ? NULL : &hxc_l_entry_hc15484524557->hxc_base, (hxc_string){ (const uint8_t *)"enemy", 5, true, NULL });
    if (!hxc_l_tmp_call_result_n56)
    {
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return false;
    }
  }
  int32_t hxc_l_g_h6e57499abda1 = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n57 = hxc_l_g_h6e57499abda1;
    int32_t hxc_l_tmp_array_length_result_n58;
    if (hxc_array_ref_length(hxc_l_drops, &hxc_l_tmp_array_length_result_n58) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n57 < hxc_l_tmp_array_length_result_n58))
    {
      break;
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeDropDefinition *hxc_l_tmp_array_get_result_n60;
    if (hxc_array_ref_get_copy(hxc_l_drops, (size_t)hxc_l_g_h6e57499abda1, &hxc_l_tmp_array_get_result_n60) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[39] = (const void *)hxc_l_tmp_array_get_result_n60;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeDropDefinition *hxc_l_entry_he2b7a813b2a5 = hxc_l_tmp_array_get_result_n60;
    hxc_l_g_h6e57499abda1 = hxc_i32_add_wrapping(hxc_l_g_h6e57499abda1, 1);
    struct hxc_array_ref *hxc_l_tmp_load_result_n62 = hxc_l_kinds;
    hxc_l_gc_roots[40] = (const void *)hxc_l_tmp_load_result_n62;
    hxc_l_gc_roots[41] = (const void *)hxc_l_entry_he2b7a813b2a5;
    hxc_l_gc_roots[42] = (const void *)(hxc_l_entry_he2b7a813b2a5 == NULL ? NULL : &hxc_l_entry_he2b7a813b2a5->hxc_base);
    bool hxc_l_tmp_call_result_n64 = hxc_caxecraft_content_RuntimeContentPack_addKind(hxc_l_reader, hxc_l_tmp_load_result_n62, hxc_l_entry_he2b7a813b2a5 == NULL ? NULL : &hxc_l_entry_he2b7a813b2a5->hxc_base, (hxc_string){ (const uint8_t *)"drop", 4, true, NULL });
    if (!hxc_l_tmp_call_result_n64)
    {
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return false;
    }
  }
  int32_t hxc_l_g_h60e4691d05ba = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n65 = hxc_l_g_h60e4691d05ba;
    int32_t hxc_l_tmp_array_length_result_n66;
    if (hxc_array_ref_length(hxc_l_effects, &hxc_l_tmp_array_length_result_n66) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n65 < hxc_l_tmp_array_length_result_n66))
    {
      break;
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEffectDefinition *hxc_l_tmp_array_get_result_n68;
    if (hxc_array_ref_get_copy(hxc_l_effects, (size_t)hxc_l_g_h60e4691d05ba, &hxc_l_tmp_array_get_result_n68) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[43] = (const void *)hxc_l_tmp_array_get_result_n68;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEffectDefinition *hxc_l_entry_h3b4f97411e99 = hxc_l_tmp_array_get_result_n68;
    hxc_l_g_h60e4691d05ba = hxc_i32_add_wrapping(hxc_l_g_h60e4691d05ba, 1);
    struct hxc_array_ref *hxc_l_tmp_load_result_n70 = hxc_l_kinds;
    hxc_l_gc_roots[44] = (const void *)hxc_l_tmp_load_result_n70;
    hxc_l_gc_roots[45] = (const void *)hxc_l_entry_h3b4f97411e99;
    hxc_l_gc_roots[46] = (const void *)(hxc_l_entry_h3b4f97411e99 == NULL ? NULL : &hxc_l_entry_h3b4f97411e99->hxc_base);
    bool hxc_l_tmp_call_result_n72 = hxc_caxecraft_content_RuntimeContentPack_addKind(hxc_l_reader, hxc_l_tmp_load_result_n70, hxc_l_entry_h3b4f97411e99 == NULL ? NULL : &hxc_l_entry_h3b4f97411e99->hxc_base, (hxc_string){ (const uint8_t *)"effect", 6, true, NULL });
    if (!hxc_l_tmp_call_result_n72)
    {
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return false;
    }
  }
  hxc_l_gc_roots[47] = (const void *)NULL;
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_air = NULL;
  int32_t hxc_l_passable = 0;
  int32_t hxc_l_g_hc10ddbded6fe = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n73 = hxc_l_g_hc10ddbded6fe;
    int32_t hxc_l_tmp_array_length_result_n74;
    if (hxc_array_ref_length(hxc_l_blocks, &hxc_l_tmp_array_length_result_n74) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n73 < hxc_l_tmp_array_length_result_n74))
    {
      break;
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_array_get_result_n76;
    if (hxc_array_ref_get_copy(hxc_l_blocks, (size_t)hxc_l_g_hc10ddbded6fe, &hxc_l_tmp_array_get_result_n76) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[48] = (const void *)hxc_l_tmp_array_get_result_n76;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_block_h4e2220a58d25 = hxc_l_tmp_array_get_result_n76;
    hxc_l_g_hc10ddbded6fe = hxc_i32_add_wrapping(hxc_l_g_hc10ddbded6fe, 1);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_load_result_n78 = hxc_l_block_h4e2220a58d25;
    hxc_l_gc_roots[49] = (const void *)hxc_l_tmp_load_result_n78;
    if (hxc_l_tmp_load_result_n78 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n79 = (*hxc_l_tmp_load_result_n78).hxc_base.hxc_id;
    if (hxc_l_airReference == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n80 = (*hxc_l_airReference).hxc_id;
    if (hxc_l_tmp_class_field_load_result_n79.data == NULL || hxc_l_tmp_class_field_load_result_n80.data == NULL ? hxc_l_tmp_class_field_load_result_n79.data == hxc_l_tmp_class_field_load_result_n80.data : hxc_l_tmp_class_field_load_result_n79.byte_length == hxc_l_tmp_class_field_load_result_n80.byte_length && (hxc_l_tmp_class_field_load_result_n79.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n79.data, hxc_l_tmp_class_field_load_result_n80.data, hxc_l_tmp_class_field_load_result_n79.byte_length) == 0))
    {
      hxc_l_gc_roots[52] = (const void *)hxc_l_block_h4e2220a58d25;
      hxc_l_air = hxc_l_block_h4e2220a58d25;
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_load_result_n83 = hxc_l_block_h4e2220a58d25;
    hxc_l_gc_roots[53] = (const void *)hxc_l_tmp_load_result_n83;
    if (hxc_l_tmp_load_result_n83 == NULL)
    {
      abort();
    }
    bool hxc_l_tmp_class_field_load_result_n84 = (*hxc_l_tmp_load_result_n83).hxc_passable;
    if (hxc_l_tmp_class_field_load_result_n84)
    {
      hxc_l_passable = hxc_i32_add_wrapping(hxc_l_passable, 1);
    }
    int32_t hxc_l_g_h6783e70d7c0d = 0;
    while (1)
    {
      int32_t hxc_l_tmp_load_result_n86 = hxc_l_g_h6783e70d7c0d;
      int32_t hxc_l_tmp_array_length_result_n87;
      if (hxc_array_ref_length(hxc_l_blocks, &hxc_l_tmp_array_length_result_n87) != HXC_STATUS_OK)
      {
        abort();
      }
      if (!(hxc_l_tmp_load_result_n86 < hxc_l_tmp_array_length_result_n87))
      {
        break;
      }
      struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_array_get_result_n89;
      if (hxc_array_ref_get_copy(hxc_l_blocks, (size_t)hxc_l_g_h6783e70d7c0d, &hxc_l_tmp_array_get_result_n89) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_gc_roots[54] = (const void *)hxc_l_tmp_array_get_result_n89;
      struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_other = hxc_l_tmp_array_get_result_n89;
      hxc_l_g_h6783e70d7c0d = hxc_i32_add_wrapping(hxc_l_g_h6783e70d7c0d, 1);
      struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_load_result_n91 = hxc_l_block_h4e2220a58d25;
      hxc_l_gc_roots[55] = (const void *)hxc_l_tmp_load_result_n91;
      struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_load_result_n92 = hxc_l_other;
      hxc_l_gc_roots[56] = (const void *)hxc_l_tmp_load_result_n92;
      bool hxc_l_tmp_short_circuit_result_n37 = hxc_l_tmp_load_result_n91 != hxc_l_tmp_load_result_n92;
      if (hxc_l_tmp_load_result_n91 != hxc_l_tmp_load_result_n92)
      {
        struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_load_result_n93 = hxc_l_block_h4e2220a58d25;
        hxc_l_gc_roots[57] = (const void *)hxc_l_tmp_load_result_n93;
        if (hxc_l_tmp_load_result_n93 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_class_field_load_result_n94 = (*hxc_l_tmp_load_result_n93).hxc_storageCode;
        struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_load_result_n95 = hxc_l_other;
        hxc_l_gc_roots[58] = (const void *)hxc_l_tmp_load_result_n95;
        if (hxc_l_tmp_load_result_n95 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_class_field_load_result_n96 = (*hxc_l_tmp_load_result_n95).hxc_storageCode;
        hxc_l_tmp_short_circuit_result_n37 = hxc_l_tmp_class_field_load_result_n94 == hxc_l_tmp_class_field_load_result_n96;
      }
      if (hxc_l_tmp_short_circuit_result_n37)
      {
        if (hxc_l_reader == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_load_result_n98 = hxc_l_other;
        hxc_l_gc_roots[59] = (const void *)hxc_l_tmp_load_result_n98;
        if (hxc_l_tmp_load_result_n98 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_class_field_load_result_n99 = (*hxc_l_tmp_load_result_n98).hxc_base.hxc_line;
        struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_load_result_n100 = hxc_l_other;
        hxc_l_gc_roots[60] = (const void *)hxc_l_tmp_load_result_n100;
        if (hxc_l_tmp_load_result_n100 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_class_field_load_result_n101 = (*hxc_l_tmp_load_result_n100).hxc_base.hxc_column;
        struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_load_result_n102 = hxc_l_other;
        hxc_l_gc_roots[61] = (const void *)hxc_l_tmp_load_result_n102;
        if (hxc_l_tmp_load_result_n102 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_class_field_load_result_n103 = (*hxc_l_tmp_load_result_n102).hxc_storageCode;
        hxc_l_tmp_instance_call_argument_2_owner_n38 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaDuplicateStorageCode, .hxc_payload.hxc_SchemaDuplicateStorageCode.hxc_code = hxc_l_tmp_class_field_load_result_n103 };
        hxc_caxecraft_content_RuntimeSchemaReader_rejectAt(hxc_l_reader, hxc_l_tmp_class_field_load_result_n99, hxc_l_tmp_class_field_load_result_n101, hxc_l_tmp_instance_call_argument_2_owner_n38);
        hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_2_owner_n38);
        if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
        {
          abort();
        }
        return false;
      }
    }
  }
  hxc_l_gc_roots[50] = (const void *)hxc_l_air;
  hxc_l_gc_roots[51] = (const void *)NULL;
  if (hxc_l_air == NULL)
  {
    if (hxc_l_reader == NULL)
    {
      abort();
    }
    if (hxc_l_airReference == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n107 = (*hxc_l_airReference).hxc_line;
    int32_t hxc_l_tmp_class_field_load_result_n108 = (*hxc_l_airReference).hxc_column;
    hxc_l_tmp_enum_payload_0_owner_n39 = (hxc_string){ (const uint8_t *)"airBlock", 8, true, NULL };
    if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n39) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_string hxc_l_tmp_enum_payload_0_owned_load_result_n109 = hxc_l_tmp_enum_payload_0_owner_n39;
    hxc_string hxc_l_tmp_class_field_load_result_n110 = (*hxc_l_airReference).hxc_id;
    hxc_l_tmp_enum_payload_1_owner_n40 = hxc_l_tmp_class_field_load_result_n110;
    if (hxc_string_retain(hxc_l_tmp_enum_payload_1_owner_n40) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_string hxc_l_tmp_enum_payload_1_owned_load_result_n111 = hxc_l_tmp_enum_payload_1_owner_n40;
    hxc_l_tmp_enum_payload_2_owner_n41 = (hxc_string){ (const uint8_t *)"block", 5, true, NULL };
    if (hxc_string_retain(hxc_l_tmp_enum_payload_2_owner_n41) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_2_owner_n42 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaUnresolvedReference, .hxc_payload.hxc_SchemaUnresolvedReference.hxc_path = hxc_l_tmp_enum_payload_0_owned_load_result_n109, .hxc_payload.hxc_SchemaUnresolvedReference.hxc_id = hxc_l_tmp_enum_payload_1_owned_load_result_n111, .hxc_payload.hxc_SchemaUnresolvedReference.hxc_expectedKind = hxc_l_tmp_enum_payload_2_owner_n41 };
    hxc_caxecraft_content_RuntimeSchemaReader_rejectAt(hxc_l_reader, hxc_l_tmp_class_field_load_result_n107, hxc_l_tmp_class_field_load_result_n108, hxc_l_tmp_instance_call_argument_2_owner_n42);
    hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_2_owner_n42);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return false;
  }
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_load_result_n115 = hxc_l_air;
  hxc_l_gc_roots[62] = (const void *)hxc_l_tmp_load_result_n115;
  if (hxc_l_tmp_load_result_n115 == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n116 = (*hxc_l_tmp_load_result_n115).hxc_storageCode;
  bool hxc_l_tmp_short_circuit_result_n43 = hxc_l_tmp_class_field_load_result_n116 != 0;
  if (!(hxc_l_tmp_class_field_load_result_n116 != 0))
  {
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_load_result_n117 = hxc_l_air;
    hxc_l_gc_roots[63] = (const void *)hxc_l_tmp_load_result_n117;
    if (hxc_l_tmp_load_result_n117 == NULL)
    {
      abort();
    }
    bool hxc_l_tmp_class_field_load_result_n118 = (*hxc_l_tmp_load_result_n117).hxc_passable;
    hxc_l_tmp_short_circuit_result_n43 = !hxc_l_tmp_class_field_load_result_n118;
  }
  bool hxc_l_tmp_short_circuit_load_result_n119 = hxc_l_tmp_short_circuit_result_n43;
  bool hxc_l_tmp_short_circuit_result_n44 = hxc_l_tmp_short_circuit_load_result_n119;
  if (!hxc_l_tmp_short_circuit_load_result_n119)
  {
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_load_result_n120 = hxc_l_air;
    hxc_l_gc_roots[64] = (const void *)hxc_l_tmp_load_result_n120;
    if (hxc_l_tmp_load_result_n120 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n121 = (*hxc_l_tmp_load_result_n120).hxc_renderProfile;
    hxc_l_tmp_short_circuit_result_n44 = !(hxc_l_tmp_class_field_load_result_n121.data != NULL && (hxc_l_tmp_class_field_load_result_n121.byte_length == (hxc_string){ (const uint8_t *)"air", 3, true, NULL }.byte_length && (hxc_l_tmp_class_field_load_result_n121.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n121.data, (hxc_string){ (const uint8_t *)"air", 3, true, NULL }.data, hxc_l_tmp_class_field_load_result_n121.byte_length) == 0)));
  }
  bool hxc_l_tmp_short_circuit_load_result_n123 = hxc_l_tmp_short_circuit_result_n44;
  bool hxc_l_tmp_short_circuit_result_n45 = hxc_l_tmp_short_circuit_load_result_n123;
  if (!hxc_l_tmp_short_circuit_load_result_n123)
  {
    hxc_l_tmp_short_circuit_result_n45 = hxc_l_passable != 1;
  }
  if (hxc_l_tmp_short_circuit_result_n45)
  {
    if (hxc_l_reader == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_load_result_n126 = hxc_l_air;
    hxc_l_gc_roots[65] = (const void *)hxc_l_tmp_load_result_n126;
    if (hxc_l_tmp_load_result_n126 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n127 = (*hxc_l_tmp_load_result_n126).hxc_base.hxc_line;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_load_result_n128 = hxc_l_air;
    hxc_l_gc_roots[66] = (const void *)hxc_l_tmp_load_result_n128;
    if (hxc_l_tmp_load_result_n128 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n129 = (*hxc_l_tmp_load_result_n128).hxc_base.hxc_column;
    hxc_l_tmp_enum_payload_0_owner_n46 = (hxc_string){ (const uint8_t *)"airBlock", 8, true, NULL };
    if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n46) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_2_owner_n47 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidInvariant, .hxc_payload.hxc_SchemaInvalidInvariant.hxc_path = hxc_l_tmp_enum_payload_0_owner_n46 };
    hxc_caxecraft_content_RuntimeSchemaReader_rejectAt(hxc_l_reader, hxc_l_tmp_class_field_load_result_n127, hxc_l_tmp_class_field_load_result_n129, hxc_l_tmp_instance_call_argument_2_owner_n47);
    hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_2_owner_n47);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return false;
  }
  struct hxc_array_ref *hxc_l_tmp_load_result_n133 = hxc_l_kinds;
  hxc_l_gc_roots[67] = (const void *)hxc_l_tmp_load_result_n133;
  if (hxc_l_defaultAquaticReference == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_class_field_load_result_n134 = (*hxc_l_defaultAquaticReference).hxc_id;
  hxc_string hxc_l_tmp_call_result_n135 = hxc_caxecraft_content_RuntimeContentPack_kindOf(hxc_l_tmp_load_result_n133, hxc_l_tmp_class_field_load_result_n134);
  hxc_l_tmp_string_equality_left_owner_n48 = hxc_l_tmp_call_result_n135;
  hxc_string hxc_l_tmp_string_equality_left_borrow_result_n136 = hxc_l_tmp_string_equality_left_owner_n48;
  if (hxc_string_release(&hxc_l_tmp_string_equality_left_owner_n48) != HXC_STATUS_OK)
  {
    abort();
  }
  if (!(hxc_l_tmp_string_equality_left_borrow_result_n136.data != NULL && (hxc_l_tmp_string_equality_left_borrow_result_n136.byte_length == (hxc_string){ (const uint8_t *)"aquatic profile", 15, true, NULL }.byte_length && (hxc_l_tmp_string_equality_left_borrow_result_n136.byte_length == 0 || memcmp(hxc_l_tmp_string_equality_left_borrow_result_n136.data, (hxc_string){ (const uint8_t *)"aquatic profile", 15, true, NULL }.data, hxc_l_tmp_string_equality_left_borrow_result_n136.byte_length) == 0))))
  {
    hxc_l_gc_roots[68] = (const void *)hxc_l_kinds;
    bool hxc_l_tmp_call_result_n139 = hxc_caxecraft_content_RuntimeContentPack_rejectReference(hxc_l_reader, hxc_l_defaultAquaticReference, (hxc_string){ (const uint8_t *)"defaultAquaticProfile", 21, true, NULL }, (hxc_string){ (const uint8_t *)"aquatic profile", 15, true, NULL }, hxc_l_kinds);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n139;
  }
  int32_t hxc_l_g_hda0dc15903ce = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n140 = hxc_l_g_hda0dc15903ce;
    int32_t hxc_l_tmp_array_length_result_n141;
    if (hxc_array_ref_length(hxc_l_blocks, &hxc_l_tmp_array_length_result_n141) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n140 < hxc_l_tmp_array_length_result_n141))
    {
      break;
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_array_get_result_n143;
    if (hxc_array_ref_get_copy(hxc_l_blocks, (size_t)hxc_l_g_hda0dc15903ce, &hxc_l_tmp_array_get_result_n143) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[69] = (const void *)hxc_l_tmp_array_get_result_n143;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_block_h1adb76a7d894 = hxc_l_tmp_array_get_result_n143;
    hxc_l_g_hda0dc15903ce = hxc_i32_add_wrapping(hxc_l_g_hda0dc15903ce, 1);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_load_result_n145 = hxc_l_block_h1adb76a7d894;
    hxc_l_gc_roots[70] = (const void *)hxc_l_tmp_load_result_n145;
    if (hxc_l_tmp_load_result_n145 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference hxc_l_tmp_class_field_load_result_n146 = (*hxc_l_tmp_load_result_n145).hxc_dropItem;
    hxc_l_gc_roots[71] = hxc_l_tmp_class_field_load_result_n146.hxc_tag == hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady ? (const void *)hxc_l_tmp_class_field_load_result_n146.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference : NULL;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference hxc_l_g_he07e233106cd = hxc_l_tmp_class_field_load_result_n146;
    hxc_l_gc_roots[72] = hxc_l_g_he07e233106cd.hxc_tag == hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady ? (const void *)hxc_l_g_he07e233106cd.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference : NULL;
    switch (hxc_l_g_he07e233106cd.hxc_tag) {
      case hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_NoRuntimeReference:
        {
          break;
        }
      case hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady:
        {
          hxc_l_gc_roots[73] = hxc_l_g_he07e233106cd.hxc_tag == hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady ? (const void *)hxc_l_g_he07e233106cd.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference : NULL;
          if (hxc_l_g_he07e233106cd.hxc_tag != hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_tmp_enum_payload_project_n149 = hxc_l_g_he07e233106cd.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference;
          hxc_l_gc_roots[74] = (const void *)hxc_l_tmp_enum_payload_project_n149;
          struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_reference_h3a1aa3820770 = hxc_l_tmp_enum_payload_project_n149;
          struct hxc_array_ref *hxc_l_tmp_load_result_n150 = hxc_l_kinds;
          hxc_l_gc_roots[75] = (const void *)hxc_l_tmp_load_result_n150;
          struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_tmp_load_result_n151 = hxc_l_reference_h3a1aa3820770;
          hxc_l_gc_roots[76] = (const void *)hxc_l_tmp_load_result_n151;
          if (hxc_l_tmp_load_result_n151 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_class_field_load_result_n152 = (*hxc_l_tmp_load_result_n151).hxc_id;
          hxc_string hxc_l_tmp_call_result_n153 = hxc_caxecraft_content_RuntimeContentPack_kindOf(hxc_l_tmp_load_result_n150, hxc_l_tmp_class_field_load_result_n152);
          hxc_l_tmp_string_equality_left_owner_n53 = hxc_l_tmp_call_result_n153;
          hxc_string hxc_l_tmp_string_equality_left_borrow_result_n154 = hxc_l_tmp_string_equality_left_owner_n53;
          bool hxc_l_tmp_short_circuit_result_n54 = !(hxc_l_tmp_string_equality_left_borrow_result_n154.data != NULL && (hxc_l_tmp_string_equality_left_borrow_result_n154.byte_length == (hxc_string){ (const uint8_t *)"item", 4, true, NULL }.byte_length && (hxc_l_tmp_string_equality_left_borrow_result_n154.byte_length == 0 || memcmp(hxc_l_tmp_string_equality_left_borrow_result_n154.data, (hxc_string){ (const uint8_t *)"item", 4, true, NULL }.data, hxc_l_tmp_string_equality_left_borrow_result_n154.byte_length) == 0)));
          if (!(hxc_l_tmp_string_equality_left_borrow_result_n154.data != NULL && (hxc_l_tmp_string_equality_left_borrow_result_n154.byte_length == (hxc_string){ (const uint8_t *)"item", 4, true, NULL }.byte_length && (hxc_l_tmp_string_equality_left_borrow_result_n154.byte_length == 0 || memcmp(hxc_l_tmp_string_equality_left_borrow_result_n154.data, (hxc_string){ (const uint8_t *)"item", 4, true, NULL }.data, hxc_l_tmp_string_equality_left_borrow_result_n154.byte_length) == 0))))
          {
            struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_tmp_load_result_n156 = hxc_l_reference_h3a1aa3820770;
            hxc_l_gc_roots[77] = (const void *)hxc_l_tmp_load_result_n156;
            hxc_l_gc_roots[78] = (const void *)hxc_l_kinds;
            bool hxc_l_tmp_call_result_n158 = hxc_caxecraft_content_RuntimeContentPack_rejectReference(hxc_l_reader, hxc_l_tmp_load_result_n156, (hxc_string){ (const uint8_t *)"block.dropItem", 14, true, NULL }, (hxc_string){ (const uint8_t *)"item", 4, true, NULL }, hxc_l_kinds);
            hxc_l_tmp_short_circuit_result_n54 = !hxc_l_tmp_call_result_n158;
          }
          bool hxc_l_tmp_short_circuit_load_result_n159 = hxc_l_tmp_short_circuit_result_n54;
          if (hxc_string_release(&hxc_l_tmp_string_equality_left_owner_n53) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_l_tmp_short_circuit_load_result_n159)
          {
            if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
            {
              abort();
            }
            return false;
          }
          break;
        }
      default:
        {
          abort();
        }
    }
  }
  int32_t hxc_l_g_h8cdca5e18edd = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n160 = hxc_l_g_h8cdca5e18edd;
    int32_t hxc_l_tmp_array_length_result_n161;
    if (hxc_array_ref_length(hxc_l_items, &hxc_l_tmp_array_length_result_n161) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n160 < hxc_l_tmp_array_length_result_n161))
    {
      break;
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_tmp_array_get_result_n163;
    if (hxc_array_ref_get_copy(hxc_l_items, (size_t)hxc_l_g_h8cdca5e18edd, &hxc_l_tmp_array_get_result_n163) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[79] = (const void *)hxc_l_tmp_array_get_result_n163;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_item_h39b975d324a3 = hxc_l_tmp_array_get_result_n163;
    hxc_l_g_h8cdca5e18edd = hxc_i32_add_wrapping(hxc_l_g_h8cdca5e18edd, 1);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_tmp_load_result_n165 = hxc_l_item_h39b975d324a3;
    hxc_l_gc_roots[80] = (const void *)hxc_l_tmp_load_result_n165;
    if (hxc_l_tmp_load_result_n165 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference hxc_l_tmp_class_field_load_result_n166 = (*hxc_l_tmp_load_result_n165).hxc_placementBlock;
    hxc_l_gc_roots[81] = hxc_l_tmp_class_field_load_result_n166.hxc_tag == hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady ? (const void *)hxc_l_tmp_class_field_load_result_n166.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference : NULL;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference hxc_l_g_h99b5a349b00a = hxc_l_tmp_class_field_load_result_n166;
    hxc_l_gc_roots[82] = hxc_l_g_h99b5a349b00a.hxc_tag == hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady ? (const void *)hxc_l_g_h99b5a349b00a.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference : NULL;
    switch (hxc_l_g_h99b5a349b00a.hxc_tag) {
      case hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_NoRuntimeReference:
        {
          break;
        }
      case hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady:
        {
          hxc_l_gc_roots[83] = hxc_l_g_h99b5a349b00a.hxc_tag == hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady ? (const void *)hxc_l_g_h99b5a349b00a.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference : NULL;
          if (hxc_l_g_h99b5a349b00a.hxc_tag != hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_tmp_enum_payload_project_n169 = hxc_l_g_h99b5a349b00a.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference;
          hxc_l_gc_roots[84] = (const void *)hxc_l_tmp_enum_payload_project_n169;
          struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_reference_ha69ec392cb79 = hxc_l_tmp_enum_payload_project_n169;
          struct hxc_array_ref *hxc_l_tmp_load_result_n170 = hxc_l_kinds;
          hxc_l_gc_roots[85] = (const void *)hxc_l_tmp_load_result_n170;
          struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_tmp_load_result_n171 = hxc_l_reference_ha69ec392cb79;
          hxc_l_gc_roots[86] = (const void *)hxc_l_tmp_load_result_n171;
          if (hxc_l_tmp_load_result_n171 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_class_field_load_result_n172 = (*hxc_l_tmp_load_result_n171).hxc_id;
          hxc_string hxc_l_tmp_call_result_n173 = hxc_caxecraft_content_RuntimeContentPack_kindOf(hxc_l_tmp_load_result_n170, hxc_l_tmp_class_field_load_result_n172);
          hxc_l_tmp_string_equality_left_owner_n59 = hxc_l_tmp_call_result_n173;
          hxc_string hxc_l_tmp_string_equality_left_borrow_result_n174 = hxc_l_tmp_string_equality_left_owner_n59;
          bool hxc_l_tmp_short_circuit_result_n60 = !(hxc_l_tmp_string_equality_left_borrow_result_n174.data != NULL && (hxc_l_tmp_string_equality_left_borrow_result_n174.byte_length == (hxc_string){ (const uint8_t *)"block", 5, true, NULL }.byte_length && (hxc_l_tmp_string_equality_left_borrow_result_n174.byte_length == 0 || memcmp(hxc_l_tmp_string_equality_left_borrow_result_n174.data, (hxc_string){ (const uint8_t *)"block", 5, true, NULL }.data, hxc_l_tmp_string_equality_left_borrow_result_n174.byte_length) == 0)));
          if (!(hxc_l_tmp_string_equality_left_borrow_result_n174.data != NULL && (hxc_l_tmp_string_equality_left_borrow_result_n174.byte_length == (hxc_string){ (const uint8_t *)"block", 5, true, NULL }.byte_length && (hxc_l_tmp_string_equality_left_borrow_result_n174.byte_length == 0 || memcmp(hxc_l_tmp_string_equality_left_borrow_result_n174.data, (hxc_string){ (const uint8_t *)"block", 5, true, NULL }.data, hxc_l_tmp_string_equality_left_borrow_result_n174.byte_length) == 0))))
          {
            struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_tmp_load_result_n176 = hxc_l_reference_ha69ec392cb79;
            hxc_l_gc_roots[87] = (const void *)hxc_l_tmp_load_result_n176;
            hxc_l_gc_roots[88] = (const void *)hxc_l_kinds;
            bool hxc_l_tmp_call_result_n178 = hxc_caxecraft_content_RuntimeContentPack_rejectReference(hxc_l_reader, hxc_l_tmp_load_result_n176, (hxc_string){ (const uint8_t *)"item.placementBlock", 19, true, NULL }, (hxc_string){ (const uint8_t *)"block", 5, true, NULL }, hxc_l_kinds);
            hxc_l_tmp_short_circuit_result_n60 = !hxc_l_tmp_call_result_n178;
          }
          bool hxc_l_tmp_short_circuit_load_result_n179 = hxc_l_tmp_short_circuit_result_n60;
          if (hxc_string_release(&hxc_l_tmp_string_equality_left_owner_n59) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_l_tmp_short_circuit_load_result_n179)
          {
            if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
            {
              abort();
            }
            return false;
          }
          break;
        }
      default:
        {
          abort();
        }
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_tmp_load_result_n180 = hxc_l_item_h39b975d324a3;
    hxc_l_gc_roots[89] = (const void *)hxc_l_tmp_load_result_n180;
    if (hxc_l_tmp_load_result_n180 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference hxc_l_tmp_class_field_load_result_n181 = (*hxc_l_tmp_load_result_n180).hxc_aquaticProfile;
    hxc_l_gc_roots[90] = hxc_l_tmp_class_field_load_result_n181.hxc_tag == hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady ? (const void *)hxc_l_tmp_class_field_load_result_n181.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference : NULL;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference hxc_l_g_h3639e8549289 = hxc_l_tmp_class_field_load_result_n181;
    hxc_l_gc_roots[91] = hxc_l_g_h3639e8549289.hxc_tag == hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady ? (const void *)hxc_l_g_h3639e8549289.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference : NULL;
    switch (hxc_l_g_h3639e8549289.hxc_tag) {
      case hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_NoRuntimeReference:
        {
          break;
        }
      case hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady:
        {
          hxc_l_gc_roots[92] = hxc_l_g_h3639e8549289.hxc_tag == hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady ? (const void *)hxc_l_g_h3639e8549289.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference : NULL;
          if (hxc_l_g_h3639e8549289.hxc_tag != hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_tmp_enum_payload_project_n184 = hxc_l_g_h3639e8549289.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference;
          hxc_l_gc_roots[93] = (const void *)hxc_l_tmp_enum_payload_project_n184;
          struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_reference_hd027de0561f9 = hxc_l_tmp_enum_payload_project_n184;
          struct hxc_array_ref *hxc_l_tmp_load_result_n185 = hxc_l_kinds;
          hxc_l_gc_roots[94] = (const void *)hxc_l_tmp_load_result_n185;
          struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_tmp_load_result_n186 = hxc_l_reference_hd027de0561f9;
          hxc_l_gc_roots[95] = (const void *)hxc_l_tmp_load_result_n186;
          if (hxc_l_tmp_load_result_n186 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_class_field_load_result_n187 = (*hxc_l_tmp_load_result_n186).hxc_id;
          hxc_string hxc_l_tmp_call_result_n188 = hxc_caxecraft_content_RuntimeContentPack_kindOf(hxc_l_tmp_load_result_n185, hxc_l_tmp_class_field_load_result_n187);
          hxc_l_tmp_string_equality_left_owner_n63 = hxc_l_tmp_call_result_n188;
          hxc_string hxc_l_tmp_string_equality_left_borrow_result_n189 = hxc_l_tmp_string_equality_left_owner_n63;
          bool hxc_l_tmp_short_circuit_result_n64 = !(hxc_l_tmp_string_equality_left_borrow_result_n189.data != NULL && (hxc_l_tmp_string_equality_left_borrow_result_n189.byte_length == (hxc_string){ (const uint8_t *)"aquatic profile", 15, true, NULL }.byte_length && (hxc_l_tmp_string_equality_left_borrow_result_n189.byte_length == 0 || memcmp(hxc_l_tmp_string_equality_left_borrow_result_n189.data, (hxc_string){ (const uint8_t *)"aquatic profile", 15, true, NULL }.data, hxc_l_tmp_string_equality_left_borrow_result_n189.byte_length) == 0)));
          if (!(hxc_l_tmp_string_equality_left_borrow_result_n189.data != NULL && (hxc_l_tmp_string_equality_left_borrow_result_n189.byte_length == (hxc_string){ (const uint8_t *)"aquatic profile", 15, true, NULL }.byte_length && (hxc_l_tmp_string_equality_left_borrow_result_n189.byte_length == 0 || memcmp(hxc_l_tmp_string_equality_left_borrow_result_n189.data, (hxc_string){ (const uint8_t *)"aquatic profile", 15, true, NULL }.data, hxc_l_tmp_string_equality_left_borrow_result_n189.byte_length) == 0))))
          {
            struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_tmp_load_result_n191 = hxc_l_reference_hd027de0561f9;
            hxc_l_gc_roots[96] = (const void *)hxc_l_tmp_load_result_n191;
            hxc_l_gc_roots[97] = (const void *)hxc_l_kinds;
            bool hxc_l_tmp_call_result_n193 = hxc_caxecraft_content_RuntimeContentPack_rejectReference(hxc_l_reader, hxc_l_tmp_load_result_n191, (hxc_string){ (const uint8_t *)"item.aquaticProfile", 19, true, NULL }, (hxc_string){ (const uint8_t *)"aquatic profile", 15, true, NULL }, hxc_l_kinds);
            hxc_l_tmp_short_circuit_result_n64 = !hxc_l_tmp_call_result_n193;
          }
          bool hxc_l_tmp_short_circuit_load_result_n194 = hxc_l_tmp_short_circuit_result_n64;
          if (hxc_string_release(&hxc_l_tmp_string_equality_left_owner_n63) != HXC_STATUS_OK)
          {
            abort();
          }
          if (hxc_l_tmp_short_circuit_load_result_n194)
          {
            if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
            {
              abort();
            }
            return false;
          }
          break;
        }
      default:
        {
          abort();
        }
    }
  }
  int32_t hxc_l_g_h452bf8126e35 = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n195 = hxc_l_g_h452bf8126e35;
    int32_t hxc_l_tmp_array_length_result_n196;
    if (hxc_array_ref_length(hxc_l_drops, &hxc_l_tmp_array_length_result_n196) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n195 < hxc_l_tmp_array_length_result_n196))
    {
      break;
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeDropDefinition *hxc_l_tmp_array_get_result_n198;
    if (hxc_array_ref_get_copy(hxc_l_drops, (size_t)hxc_l_g_h452bf8126e35, &hxc_l_tmp_array_get_result_n198) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[98] = (const void *)hxc_l_tmp_array_get_result_n198;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeDropDefinition *hxc_l_drop = hxc_l_tmp_array_get_result_n198;
    hxc_l_g_h452bf8126e35 = hxc_i32_add_wrapping(hxc_l_g_h452bf8126e35, 1);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeDropDefinition *hxc_l_tmp_load_result_n200 = hxc_l_drop;
    hxc_l_gc_roots[99] = (const void *)hxc_l_tmp_load_result_n200;
    if (hxc_l_tmp_load_result_n200 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_tmp_class_field_load_result_n201 = (*hxc_l_tmp_load_result_n200).hxc_item;
    hxc_l_gc_roots[100] = (const void *)hxc_l_tmp_class_field_load_result_n201;
    if (hxc_l_tmp_class_field_load_result_n201 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n202 = (*hxc_l_tmp_class_field_load_result_n201).hxc_id;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_tmp_call_result_n203 = hxc_caxecraft_content_RuntimeContentPack_findItemDefinition(hxc_l_items, hxc_l_tmp_class_field_load_result_n202);
    hxc_l_gc_roots[101] = (const void *)hxc_l_tmp_call_result_n203;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_item_h1a3656c41a6e = hxc_l_tmp_call_result_n203;
    hxc_l_gc_roots[102] = (const void *)hxc_l_item_h1a3656c41a6e;
    hxc_l_gc_roots[103] = (const void *)NULL;
    if (hxc_l_item_h1a3656c41a6e == NULL)
    {
      struct hxc_caxecraft_content_RuntimeContentPack_RuntimeDropDefinition *hxc_l_tmp_load_result_n205 = hxc_l_drop;
      hxc_l_gc_roots[104] = (const void *)hxc_l_tmp_load_result_n205;
      if (hxc_l_tmp_load_result_n205 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_tmp_class_field_load_result_n206 = (*hxc_l_tmp_load_result_n205).hxc_item;
      hxc_l_gc_roots[105] = (const void *)hxc_l_tmp_class_field_load_result_n206;
      hxc_l_gc_roots[106] = (const void *)hxc_l_kinds;
      bool hxc_l_tmp_call_result_n208 = hxc_caxecraft_content_RuntimeContentPack_rejectReference(hxc_l_reader, hxc_l_tmp_class_field_load_result_n206, (hxc_string){ (const uint8_t *)"drop.item", 9, true, NULL }, (hxc_string){ (const uint8_t *)"item", 4, true, NULL }, hxc_l_kinds);
      if (!hxc_l_tmp_call_result_n208)
      {
        if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
        {
          abort();
        }
        return false;
      }
    }
    else
    {
      struct hxc_caxecraft_content_RuntimeContentPack_RuntimeDropDefinition *hxc_l_tmp_load_result_n209 = hxc_l_drop;
      hxc_l_gc_roots[107] = (const void *)hxc_l_tmp_load_result_n209;
      if (hxc_l_tmp_load_result_n209 == NULL)
      {
        abort();
      }
      int32_t hxc_l_tmp_class_field_load_result_n210 = (*hxc_l_tmp_load_result_n209).hxc_quantity;
      struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_tmp_load_result_n211 = hxc_l_item_h1a3656c41a6e;
      hxc_l_gc_roots[108] = (const void *)hxc_l_tmp_load_result_n211;
      if (hxc_l_tmp_load_result_n211 == NULL)
      {
        abort();
      }
      int32_t hxc_l_tmp_class_field_load_result_n212 = (*hxc_l_tmp_load_result_n211).hxc_maximumStack;
      if (hxc_l_tmp_class_field_load_result_n210 > hxc_l_tmp_class_field_load_result_n212)
      {
        if (hxc_l_reader == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_content_RuntimeContentPack_RuntimeDropDefinition *hxc_l_tmp_load_result_n213 = hxc_l_drop;
        hxc_l_gc_roots[109] = (const void *)hxc_l_tmp_load_result_n213;
        if (hxc_l_tmp_load_result_n213 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_class_field_load_result_n214 = (*hxc_l_tmp_load_result_n213).hxc_base.hxc_line;
        struct hxc_caxecraft_content_RuntimeContentPack_RuntimeDropDefinition *hxc_l_tmp_load_result_n215 = hxc_l_drop;
        hxc_l_gc_roots[110] = (const void *)hxc_l_tmp_load_result_n215;
        if (hxc_l_tmp_load_result_n215 == NULL)
        {
          abort();
        }
        int32_t hxc_l_tmp_class_field_load_result_n216 = (*hxc_l_tmp_load_result_n215).hxc_base.hxc_column;
        hxc_l_tmp_enum_payload_0_owner_n68 = (hxc_string){ (const uint8_t *)"drop.quantity", 13, true, NULL };
        if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n68) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_instance_call_argument_2_owner_n69 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidInvariant, .hxc_payload.hxc_SchemaInvalidInvariant.hxc_path = hxc_l_tmp_enum_payload_0_owner_n68 };
        hxc_caxecraft_content_RuntimeSchemaReader_rejectAt(hxc_l_reader, hxc_l_tmp_class_field_load_result_n214, hxc_l_tmp_class_field_load_result_n216, hxc_l_tmp_instance_call_argument_2_owner_n69);
        hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_2_owner_n69);
        if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
        {
          abort();
        }
        return false;
      }
    }
  }
  int32_t hxc_l_g_h7ffe6a478453 = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n220 = hxc_l_g_h7ffe6a478453;
    int32_t hxc_l_tmp_array_length_result_n221;
    if (hxc_array_ref_length(hxc_l_enemies, &hxc_l_tmp_array_length_result_n221) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n220 < hxc_l_tmp_array_length_result_n221))
    {
      break;
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_tmp_array_get_result_n223;
    if (hxc_array_ref_get_copy(hxc_l_enemies, (size_t)hxc_l_g_h7ffe6a478453, &hxc_l_tmp_array_get_result_n223) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[111] = (const void *)hxc_l_tmp_array_get_result_n223;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_enemy = hxc_l_tmp_array_get_result_n223;
    hxc_l_g_h7ffe6a478453 = hxc_i32_add_wrapping(hxc_l_g_h7ffe6a478453, 1);
    struct hxc_array_ref *hxc_l_tmp_load_result_n225 = hxc_l_kinds;
    hxc_l_gc_roots[112] = (const void *)hxc_l_tmp_load_result_n225;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_tmp_load_result_n226 = hxc_l_enemy;
    hxc_l_gc_roots[113] = (const void *)hxc_l_tmp_load_result_n226;
    if (hxc_l_tmp_load_result_n226 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_tmp_class_field_load_result_n227 = (*hxc_l_tmp_load_result_n226).hxc_drop;
    hxc_l_gc_roots[114] = (const void *)hxc_l_tmp_class_field_load_result_n227;
    if (hxc_l_tmp_class_field_load_result_n227 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n228 = (*hxc_l_tmp_class_field_load_result_n227).hxc_id;
    hxc_string hxc_l_tmp_call_result_n229 = hxc_caxecraft_content_RuntimeContentPack_kindOf(hxc_l_tmp_load_result_n225, hxc_l_tmp_class_field_load_result_n228);
    hxc_l_tmp_string_equality_left_owner_n72 = hxc_l_tmp_call_result_n229;
    hxc_string hxc_l_tmp_string_equality_left_borrow_result_n230 = hxc_l_tmp_string_equality_left_owner_n72;
    bool hxc_l_tmp_short_circuit_result_n73 = !(hxc_l_tmp_string_equality_left_borrow_result_n230.data != NULL && (hxc_l_tmp_string_equality_left_borrow_result_n230.byte_length == (hxc_string){ (const uint8_t *)"drop", 4, true, NULL }.byte_length && (hxc_l_tmp_string_equality_left_borrow_result_n230.byte_length == 0 || memcmp(hxc_l_tmp_string_equality_left_borrow_result_n230.data, (hxc_string){ (const uint8_t *)"drop", 4, true, NULL }.data, hxc_l_tmp_string_equality_left_borrow_result_n230.byte_length) == 0)));
    if (!(hxc_l_tmp_string_equality_left_borrow_result_n230.data != NULL && (hxc_l_tmp_string_equality_left_borrow_result_n230.byte_length == (hxc_string){ (const uint8_t *)"drop", 4, true, NULL }.byte_length && (hxc_l_tmp_string_equality_left_borrow_result_n230.byte_length == 0 || memcmp(hxc_l_tmp_string_equality_left_borrow_result_n230.data, (hxc_string){ (const uint8_t *)"drop", 4, true, NULL }.data, hxc_l_tmp_string_equality_left_borrow_result_n230.byte_length) == 0))))
    {
      struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_tmp_load_result_n232 = hxc_l_enemy;
      hxc_l_gc_roots[115] = (const void *)hxc_l_tmp_load_result_n232;
      if (hxc_l_tmp_load_result_n232 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_tmp_class_field_load_result_n233 = (*hxc_l_tmp_load_result_n232).hxc_drop;
      hxc_l_gc_roots[116] = (const void *)hxc_l_tmp_class_field_load_result_n233;
      hxc_l_gc_roots[117] = (const void *)hxc_l_kinds;
      bool hxc_l_tmp_call_result_n235 = hxc_caxecraft_content_RuntimeContentPack_rejectReference(hxc_l_reader, hxc_l_tmp_class_field_load_result_n233, (hxc_string){ (const uint8_t *)"enemy.drop", 10, true, NULL }, (hxc_string){ (const uint8_t *)"drop", 4, true, NULL }, hxc_l_kinds);
      hxc_l_tmp_short_circuit_result_n73 = !hxc_l_tmp_call_result_n235;
    }
    bool hxc_l_tmp_short_circuit_load_result_n236 = hxc_l_tmp_short_circuit_result_n73;
    if (hxc_string_release(&hxc_l_tmp_string_equality_left_owner_n72) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_l_tmp_short_circuit_load_result_n236)
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

bool hxc_caxecraft_content_RuntimeContentPack_validatePair(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, hxc_string hxc_l_path, struct hxc_caxecraft_content_RuntimeContentPack_RuntimeLocatedId *hxc_l_previous, struct hxc_caxecraft_content_RuntimeContentPack_RuntimeLocatedId *hxc_l_current)
{
  const void *hxc_l_gc_roots[2] = { (const void *)hxc_l_previous, (const void *)hxc_l_current };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 2, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_enum_payload_0_owner_n5 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_2_owner_n6 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_2_owner_n9 = { 0 };
  if (hxc_l_previous == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_previous).hxc_id;
  if (hxc_l_current == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_class_field_load_result_n1 = (*hxc_l_current).hxc_id;
  int32_t hxc_l_tmp_call_result_n2 = hxc_caxecraft_content_RuntimeSchemaReader_compareUtf8(hxc_l_tmp_class_field_load_result_n0, hxc_l_tmp_class_field_load_result_n1);
  int32_t hxc_l_comparison = hxc_l_tmp_call_result_n2;
  if (hxc_l_comparison > 0)
  {
    if (hxc_l_reader == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n4 = (*hxc_l_current).hxc_line;
    int32_t hxc_l_tmp_class_field_load_result_n5 = (*hxc_l_current).hxc_column;
    hxc_l_tmp_enum_payload_0_owner_n5 = hxc_l_path;
    if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_2_owner_n6 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaNonCanonicalOrder, .hxc_payload.hxc_SchemaNonCanonicalOrder.hxc_path = hxc_l_tmp_enum_payload_0_owner_n5 };
    hxc_caxecraft_content_RuntimeSchemaReader_rejectAt(hxc_l_reader, hxc_l_tmp_class_field_load_result_n4, hxc_l_tmp_class_field_load_result_n5, hxc_l_tmp_instance_call_argument_2_owner_n6);
    hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_2_owner_n6);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return false;
  }
  if (!(hxc_l_comparison == 0))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_l_reader == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_class_field_load_result_n10 = (*hxc_l_current).hxc_line;
  int32_t hxc_l_tmp_class_field_load_result_n11 = (*hxc_l_current).hxc_column;
  hxc_string hxc_l_tmp_enum_payload_0_owner_n7 = hxc_l_path;
  if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n7) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_enum_payload_0_owned_load_result_n12 = hxc_l_tmp_enum_payload_0_owner_n7;
  hxc_string hxc_l_tmp_class_field_load_result_n13 = (*hxc_l_current).hxc_id;
  hxc_string hxc_l_tmp_enum_payload_1_owner_n8 = hxc_l_tmp_class_field_load_result_n13;
  if (hxc_string_retain(hxc_l_tmp_enum_payload_1_owner_n8) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_instance_call_argument_2_owner_n9 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaDuplicateId, .hxc_payload.hxc_SchemaDuplicateId.hxc_path = hxc_l_tmp_enum_payload_0_owned_load_result_n12, .hxc_payload.hxc_SchemaDuplicateId.hxc_id = hxc_l_tmp_enum_payload_1_owner_n8 };
  hxc_caxecraft_content_RuntimeSchemaReader_rejectAt(hxc_l_reader, hxc_l_tmp_class_field_load_result_n10, hxc_l_tmp_class_field_load_result_n11, hxc_l_tmp_instance_call_argument_2_owner_n9);
  hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_2_owner_n9);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return false;
}

int32_t hxc_caxecraft_content_RuntimeContentRegistry_blockStorageCode(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[5] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 5, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeContentRegistry_findBlock(hxc_l_self, hxc_l_id);
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_instance_call_result_n0;
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_block = hxc_l_tmp_instance_call_result_n0;
  hxc_l_gc_roots[2] = (const void *)hxc_l_block;
  hxc_l_gc_roots[3] = (const void *)NULL;
  if (!(hxc_l_block == NULL))
  {
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_load_result_n2 = hxc_l_block;
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n2;
    if (hxc_l_tmp_load_result_n2 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n3 = (*hxc_l_tmp_load_result_n2).hxc_storageCode;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_class_field_load_result_n3;
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return -1;
}

struct hxc_caxecraft_domain_AquaticProfile hxc_caxecraft_content_RuntimeContentRegistry_defaultAquaticProfile(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self)
{
  const void *hxc_l_gc_roots[5] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 5, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_defaultAquaticProfileId;
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition *hxc_l_tmp_instance_call_result_n1 = hxc_caxecraft_content_RuntimeContentRegistry_findAquatic(hxc_l_self, hxc_l_tmp_class_field_load_result_n0);
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_instance_call_result_n1;
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition *hxc_l_definition = hxc_l_tmp_instance_call_result_n1;
  hxc_l_gc_roots[2] = (const void *)hxc_l_definition;
  hxc_l_gc_roots[3] = (const void *)NULL;
  if (!(hxc_l_definition == NULL))
  {
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition *hxc_l_tmp_load_result_n4 = hxc_l_definition;
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n4;
    if (hxc_l_tmp_load_result_n4 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_instance_call_result_n5 = hxc_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition_profile(hxc_l_tmp_load_result_n4);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_instance_call_result_n5;
  }
  struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_call_result_n3 = hxc_caxecraft_domain_Aquatics_profile(1, 1, 0.0, 0.0, 0.0, 0.0, 0.0, 1, false, false);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_call_result_n3;
}

hxc_string hxc_caxecraft_content_RuntimeContentRegistry_defaultEditorBlockId(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self)
{
  const void *hxc_l_gc_roots[7] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 7, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_returned_string_owner_n4 = { 0 };
  int32_t hxc_l_g = 0;
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_blocks;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  struct hxc_array_ref *hxc_l_g1 = hxc_l_tmp_class_field_load_result_n0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_g;
    hxc_l_gc_roots[2] = (const void *)hxc_l_g1;
    int32_t hxc_l_tmp_array_length_result_n3;
    if (hxc_array_ref_length(hxc_l_g1, &hxc_l_tmp_array_length_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n1 < hxc_l_tmp_array_length_result_n3))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n4 = hxc_l_g1;
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_load_result_n4;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_array_get_result_n6;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n4, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_array_get_result_n6;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_block = hxc_l_tmp_array_get_result_n6;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_load_result_n8 = hxc_l_block;
    hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_load_result_n8;
    if (hxc_l_tmp_load_result_n8 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n9 = (*hxc_l_tmp_load_result_n8).hxc_base.hxc_id;
    hxc_string hxc_l_tmp_class_field_load_result_n10 = (*hxc_l_self).hxc_airBlock;
    if (!(hxc_l_tmp_class_field_load_result_n9.data == NULL || hxc_l_tmp_class_field_load_result_n10.data == NULL ? hxc_l_tmp_class_field_load_result_n9.data == hxc_l_tmp_class_field_load_result_n10.data : hxc_l_tmp_class_field_load_result_n9.byte_length == hxc_l_tmp_class_field_load_result_n10.byte_length && (hxc_l_tmp_class_field_load_result_n9.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n9.data, hxc_l_tmp_class_field_load_result_n10.data, hxc_l_tmp_class_field_load_result_n9.byte_length) == 0)))
    {
      struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_load_result_n12 = hxc_l_block;
      hxc_l_gc_roots[6] = (const void *)hxc_l_tmp_load_result_n12;
      if (hxc_l_tmp_load_result_n12 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_class_field_load_result_n13 = (*hxc_l_tmp_load_result_n12).hxc_base.hxc_id;
      hxc_l_tmp_returned_string_owner_n4 = hxc_l_tmp_class_field_load_result_n13;
      if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n4) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_tmp_returned_string_owner_n4;
    }
  }
  hxc_string hxc_l_tmp_class_field_load_result_n15 = (*hxc_l_self).hxc_airBlock;
  hxc_string hxc_l_tmp_returned_string_owner_n5 = hxc_l_tmp_class_field_load_result_n15;
  if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_returned_string_owner_n5;
}

int32_t hxc_caxecraft_content_RuntimeContentRegistry_dropQuantityById(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[7] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 7, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g = 0;
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_drops;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  struct hxc_array_ref *hxc_l_g1 = hxc_l_tmp_class_field_load_result_n0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_g;
    hxc_l_gc_roots[2] = (const void *)hxc_l_g1;
    int32_t hxc_l_tmp_array_length_result_n3;
    if (hxc_array_ref_length(hxc_l_g1, &hxc_l_tmp_array_length_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n1 < hxc_l_tmp_array_length_result_n3))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n4 = hxc_l_g1;
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_load_result_n4;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeDropDefinition *hxc_l_tmp_array_get_result_n6;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n4, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_array_get_result_n6;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeDropDefinition *hxc_l_drop = hxc_l_tmp_array_get_result_n6;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeDropDefinition *hxc_l_tmp_load_result_n8 = hxc_l_drop;
    hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_load_result_n8;
    if (hxc_l_tmp_load_result_n8 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n9 = (*hxc_l_tmp_load_result_n8).hxc_base.hxc_id;
    if (hxc_l_tmp_class_field_load_result_n9.data == NULL || hxc_l_id.data == NULL ? hxc_l_tmp_class_field_load_result_n9.data == hxc_l_id.data : hxc_l_tmp_class_field_load_result_n9.byte_length == hxc_l_id.byte_length && (hxc_l_tmp_class_field_load_result_n9.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n9.data, hxc_l_id.data, hxc_l_tmp_class_field_load_result_n9.byte_length) == 0))
    {
      struct hxc_caxecraft_content_RuntimeContentPack_RuntimeDropDefinition *hxc_l_tmp_load_result_n11 = hxc_l_drop;
      hxc_l_gc_roots[6] = (const void *)hxc_l_tmp_load_result_n11;
      if (hxc_l_tmp_load_result_n11 == NULL)
      {
        abort();
      }
      int32_t hxc_l_tmp_class_field_load_result_n12 = (*hxc_l_tmp_load_result_n11).hxc_quantity;
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_tmp_class_field_load_result_n12;
    }
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return 0;
}

struct hxc_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition *hxc_caxecraft_content_RuntimeContentRegistry_findAquatic(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[8] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 8, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g = 0;
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_aquaticProfiles;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  struct hxc_array_ref *hxc_l_g1 = hxc_l_tmp_class_field_load_result_n0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_g;
    hxc_l_gc_roots[2] = (const void *)hxc_l_g1;
    int32_t hxc_l_tmp_array_length_result_n3;
    if (hxc_array_ref_length(hxc_l_g1, &hxc_l_tmp_array_length_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n1 < hxc_l_tmp_array_length_result_n3))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n4 = hxc_l_g1;
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_load_result_n4;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition *hxc_l_tmp_array_get_result_n6;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n4, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_array_get_result_n6;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition *hxc_l_profile = hxc_l_tmp_array_get_result_n6;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition *hxc_l_tmp_load_result_n8 = hxc_l_profile;
    hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_load_result_n8;
    if (hxc_l_tmp_load_result_n8 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n9 = (*hxc_l_tmp_load_result_n8).hxc_base.hxc_id;
    if (hxc_l_tmp_class_field_load_result_n9.data == NULL || hxc_l_id.data == NULL ? hxc_l_tmp_class_field_load_result_n9.data == hxc_l_id.data : hxc_l_tmp_class_field_load_result_n9.byte_length == hxc_l_id.byte_length && (hxc_l_tmp_class_field_load_result_n9.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n9.data, hxc_l_id.data, hxc_l_tmp_class_field_load_result_n9.byte_length) == 0))
    {
      hxc_l_gc_roots[7] = (const void *)hxc_l_profile;
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_profile;
    }
  }
  hxc_l_gc_roots[6] = (const void *)NULL;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return NULL;
}

struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_caxecraft_content_RuntimeContentRegistry_findBlock(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[8] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 8, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g = 0;
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_blocks;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  struct hxc_array_ref *hxc_l_g1 = hxc_l_tmp_class_field_load_result_n0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_g;
    hxc_l_gc_roots[2] = (const void *)hxc_l_g1;
    int32_t hxc_l_tmp_array_length_result_n3;
    if (hxc_array_ref_length(hxc_l_g1, &hxc_l_tmp_array_length_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n1 < hxc_l_tmp_array_length_result_n3))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n4 = hxc_l_g1;
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_load_result_n4;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_array_get_result_n6;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n4, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_array_get_result_n6;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_block = hxc_l_tmp_array_get_result_n6;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_load_result_n8 = hxc_l_block;
    hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_load_result_n8;
    if (hxc_l_tmp_load_result_n8 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n9 = (*hxc_l_tmp_load_result_n8).hxc_base.hxc_id;
    if (hxc_l_tmp_class_field_load_result_n9.data == NULL || hxc_l_id.data == NULL ? hxc_l_tmp_class_field_load_result_n9.data == hxc_l_id.data : hxc_l_tmp_class_field_load_result_n9.byte_length == hxc_l_id.byte_length && (hxc_l_tmp_class_field_load_result_n9.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n9.data, hxc_l_id.data, hxc_l_tmp_class_field_load_result_n9.byte_length) == 0))
    {
      hxc_l_gc_roots[7] = (const void *)hxc_l_block;
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_block;
    }
  }
  hxc_l_gc_roots[6] = (const void *)NULL;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return NULL;
}

struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_caxecraft_content_RuntimeContentRegistry_findEnemy(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[8] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 8, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g = 0;
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_enemies;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  struct hxc_array_ref *hxc_l_g1 = hxc_l_tmp_class_field_load_result_n0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_g;
    hxc_l_gc_roots[2] = (const void *)hxc_l_g1;
    int32_t hxc_l_tmp_array_length_result_n3;
    if (hxc_array_ref_length(hxc_l_g1, &hxc_l_tmp_array_length_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n1 < hxc_l_tmp_array_length_result_n3))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n4 = hxc_l_g1;
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_load_result_n4;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_tmp_array_get_result_n6;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n4, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_array_get_result_n6;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_enemy = hxc_l_tmp_array_get_result_n6;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_tmp_load_result_n8 = hxc_l_enemy;
    hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_load_result_n8;
    if (hxc_l_tmp_load_result_n8 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n9 = (*hxc_l_tmp_load_result_n8).hxc_base.hxc_id;
    if (hxc_l_tmp_class_field_load_result_n9.data == NULL || hxc_l_id.data == NULL ? hxc_l_tmp_class_field_load_result_n9.data == hxc_l_id.data : hxc_l_tmp_class_field_load_result_n9.byte_length == hxc_l_id.byte_length && (hxc_l_tmp_class_field_load_result_n9.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n9.data, hxc_l_id.data, hxc_l_tmp_class_field_load_result_n9.byte_length) == 0))
    {
      hxc_l_gc_roots[7] = (const void *)hxc_l_enemy;
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_enemy;
    }
  }
  hxc_l_gc_roots[6] = (const void *)NULL;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return NULL;
}

struct hxc_caxecraft_content_RuntimeContentPack_RuntimeFluidDefinition *hxc_caxecraft_content_RuntimeContentRegistry_findFluid(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[8] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 8, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g = 0;
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_fluids;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  struct hxc_array_ref *hxc_l_g1 = hxc_l_tmp_class_field_load_result_n0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_g;
    hxc_l_gc_roots[2] = (const void *)hxc_l_g1;
    int32_t hxc_l_tmp_array_length_result_n3;
    if (hxc_array_ref_length(hxc_l_g1, &hxc_l_tmp_array_length_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n1 < hxc_l_tmp_array_length_result_n3))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n4 = hxc_l_g1;
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_load_result_n4;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeFluidDefinition *hxc_l_tmp_array_get_result_n6;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n4, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_array_get_result_n6;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeFluidDefinition *hxc_l_fluid = hxc_l_tmp_array_get_result_n6;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeFluidDefinition *hxc_l_tmp_load_result_n8 = hxc_l_fluid;
    hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_load_result_n8;
    if (hxc_l_tmp_load_result_n8 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n9 = (*hxc_l_tmp_load_result_n8).hxc_base.hxc_id;
    if (hxc_l_tmp_class_field_load_result_n9.data == NULL || hxc_l_id.data == NULL ? hxc_l_tmp_class_field_load_result_n9.data == hxc_l_id.data : hxc_l_tmp_class_field_load_result_n9.byte_length == hxc_l_id.byte_length && (hxc_l_tmp_class_field_load_result_n9.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n9.data, hxc_l_id.data, hxc_l_tmp_class_field_load_result_n9.byte_length) == 0))
    {
      hxc_l_gc_roots[7] = (const void *)hxc_l_fluid;
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_fluid;
    }
  }
  hxc_l_gc_roots[6] = (const void *)NULL;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return NULL;
}

struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_caxecraft_content_RuntimeContentRegistry_findItem(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[8] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 8, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g = 0;
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_items;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  struct hxc_array_ref *hxc_l_g1 = hxc_l_tmp_class_field_load_result_n0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_g;
    hxc_l_gc_roots[2] = (const void *)hxc_l_g1;
    int32_t hxc_l_tmp_array_length_result_n3;
    if (hxc_array_ref_length(hxc_l_g1, &hxc_l_tmp_array_length_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n1 < hxc_l_tmp_array_length_result_n3))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n4 = hxc_l_g1;
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_load_result_n4;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_tmp_array_get_result_n6;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n4, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_array_get_result_n6;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_item = hxc_l_tmp_array_get_result_n6;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_tmp_load_result_n8 = hxc_l_item;
    hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_load_result_n8;
    if (hxc_l_tmp_load_result_n8 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n9 = (*hxc_l_tmp_load_result_n8).hxc_base.hxc_id;
    if (hxc_l_tmp_class_field_load_result_n9.data == NULL || hxc_l_id.data == NULL ? hxc_l_tmp_class_field_load_result_n9.data == hxc_l_id.data : hxc_l_tmp_class_field_load_result_n9.byte_length == hxc_l_id.byte_length && (hxc_l_tmp_class_field_load_result_n9.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n9.data, hxc_l_id.data, hxc_l_tmp_class_field_load_result_n9.byte_length) == 0))
    {
      hxc_l_gc_roots[7] = (const void *)hxc_l_item;
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_item;
    }
  }
  hxc_l_gc_roots[6] = (const void *)NULL;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return NULL;
}

struct hxc_caxecraft_content_RuntimeContentPack_RuntimeNpcDefinition *hxc_caxecraft_content_RuntimeContentRegistry_findNpc(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[8] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 8, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g = 0;
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_npcs;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  struct hxc_array_ref *hxc_l_g1 = hxc_l_tmp_class_field_load_result_n0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_g;
    hxc_l_gc_roots[2] = (const void *)hxc_l_g1;
    int32_t hxc_l_tmp_array_length_result_n3;
    if (hxc_array_ref_length(hxc_l_g1, &hxc_l_tmp_array_length_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n1 < hxc_l_tmp_array_length_result_n3))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n4 = hxc_l_g1;
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_load_result_n4;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeNpcDefinition *hxc_l_tmp_array_get_result_n6;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n4, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_array_get_result_n6;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeNpcDefinition *hxc_l_npc = hxc_l_tmp_array_get_result_n6;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeNpcDefinition *hxc_l_tmp_load_result_n8 = hxc_l_npc;
    hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_load_result_n8;
    if (hxc_l_tmp_load_result_n8 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n9 = (*hxc_l_tmp_load_result_n8).hxc_base.hxc_id;
    if (hxc_l_tmp_class_field_load_result_n9.data == NULL || hxc_l_id.data == NULL ? hxc_l_tmp_class_field_load_result_n9.data == hxc_l_id.data : hxc_l_tmp_class_field_load_result_n9.byte_length == hxc_l_id.byte_length && (hxc_l_tmp_class_field_load_result_n9.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n9.data, hxc_l_id.data, hxc_l_tmp_class_field_load_result_n9.byte_length) == 0))
    {
      hxc_l_gc_roots[7] = (const void *)hxc_l_npc;
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_npc;
    }
  }
  hxc_l_gc_roots[6] = (const void *)NULL;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return NULL;
}

bool hxc_caxecraft_content_RuntimeContentRegistry_hasBlock(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
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
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeContentRegistry_findBlock(hxc_l_self, hxc_l_id);
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_instance_call_result_n0;
  hxc_l_gc_roots[2] = (const void *)NULL;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_instance_call_result_n0 != NULL;
}

bool hxc_caxecraft_content_RuntimeContentRegistry_hasEffect(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[6] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 6, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g = 0;
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_effects;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  struct hxc_array_ref *hxc_l_g1 = hxc_l_tmp_class_field_load_result_n0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_g;
    hxc_l_gc_roots[2] = (const void *)hxc_l_g1;
    int32_t hxc_l_tmp_array_length_result_n3;
    if (hxc_array_ref_length(hxc_l_g1, &hxc_l_tmp_array_length_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n1 < hxc_l_tmp_array_length_result_n3))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n4 = hxc_l_g1;
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_load_result_n4;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEffectDefinition *hxc_l_tmp_array_get_result_n6;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n4, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_array_get_result_n6;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEffectDefinition *hxc_l_effect = hxc_l_tmp_array_get_result_n6;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEffectDefinition *hxc_l_tmp_load_result_n8 = hxc_l_effect;
    hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_load_result_n8;
    if (hxc_l_tmp_load_result_n8 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n9 = (*hxc_l_tmp_load_result_n8).hxc_base.hxc_id;
    if (hxc_l_tmp_class_field_load_result_n9.data == NULL || hxc_l_id.data == NULL ? hxc_l_tmp_class_field_load_result_n9.data == hxc_l_id.data : hxc_l_tmp_class_field_load_result_n9.byte_length == hxc_l_id.byte_length && (hxc_l_tmp_class_field_load_result_n9.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n9.data, hxc_l_id.data, hxc_l_tmp_class_field_load_result_n9.byte_length) == 0))
    {
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return true;
    }
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return false;
}

bool hxc_caxecraft_content_RuntimeContentRegistry_hasEntity(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
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
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeContentRegistry_findEnemy(hxc_l_self, hxc_l_id);
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_instance_call_result_n0;
  hxc_l_gc_roots[2] = (const void *)NULL;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_instance_call_result_n0 != NULL;
}

bool hxc_caxecraft_content_RuntimeContentRegistry_hasFluid(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
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
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeFluidDefinition *hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeContentRegistry_findFluid(hxc_l_self, hxc_l_id);
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_instance_call_result_n0;
  hxc_l_gc_roots[2] = (const void *)NULL;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_instance_call_result_n0 != NULL;
}

bool hxc_caxecraft_content_RuntimeContentRegistry_hasItem(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
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
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeContentRegistry_findItem(hxc_l_self, hxc_l_id);
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_instance_call_result_n0;
  hxc_l_gc_roots[2] = (const void *)NULL;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_instance_call_result_n0 != NULL;
}

bool hxc_caxecraft_content_RuntimeContentRegistry_hasNpc(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
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
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeNpcDefinition *hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeContentRegistry_findNpc(hxc_l_self, hxc_l_id);
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_instance_call_result_n0;
  hxc_l_gc_roots[2] = (const void *)NULL;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_instance_call_result_n0 != NULL;
}

bool hxc_caxecraft_content_RuntimeContentRegistry_hasPrefab(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  (void)hxc_l_id;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return false;
}

bool hxc_caxecraft_content_RuntimeContentRegistry_hasSignal(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  (void)hxc_l_id;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return false;
}

bool hxc_caxecraft_content_RuntimeContentRegistry_hasState(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  (void)hxc_l_id;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return false;
}

bool hxc_caxecraft_content_RuntimeContentRegistry_hasStatefulObject(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  (void)hxc_l_id;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return false;
}

bool hxc_caxecraft_content_RuntimeContentRegistry_isAirBlock(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
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
  hxc_string hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_airBlock;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_id.data == NULL || hxc_l_tmp_class_field_load_result_n0.data == NULL ? hxc_l_id.data == hxc_l_tmp_class_field_load_result_n0.data : hxc_l_id.byte_length == hxc_l_tmp_class_field_load_result_n0.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, hxc_l_tmp_class_field_load_result_n0.data, hxc_l_id.byte_length) == 0);
}

struct hxc_caxecraft_domain_AquaticProfile hxc_caxecraft_content_RuntimeContentRegistry_itemAquaticProfile(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, int32_t hxc_l_code)
{
  const void *hxc_l_gc_roots[13] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 13, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_tmp_short_circuit_result_n2 = hxc_l_code >= 0;
  if (hxc_l_code >= 0)
  {
    if (hxc_l_self == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_items;
    hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
    int32_t hxc_l_tmp_array_length_result_n1;
    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n0, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n2 = hxc_l_code < hxc_l_tmp_array_length_result_n1;
  }
  if (hxc_l_tmp_short_circuit_result_n2)
  {
    if (hxc_l_self == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n3 = (*hxc_l_self).hxc_items;
    hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_class_field_load_result_n3;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_tmp_array_get_result_n4;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n3, (size_t)hxc_l_code, &hxc_l_tmp_array_get_result_n4) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_array_get_result_n4;
    if (hxc_l_tmp_array_get_result_n4 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference hxc_l_tmp_class_field_load_result_n5 = (*hxc_l_tmp_array_get_result_n4).hxc_aquaticProfile;
    hxc_l_gc_roots[4] = hxc_l_tmp_class_field_load_result_n5.hxc_tag == hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady ? (const void *)hxc_l_tmp_class_field_load_result_n5.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference : NULL;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference hxc_l_g = hxc_l_tmp_class_field_load_result_n5;
    hxc_l_gc_roots[5] = hxc_l_g.hxc_tag == hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady ? (const void *)hxc_l_g.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference : NULL;
    switch (hxc_l_g.hxc_tag) {
      case hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_NoRuntimeReference:
        {
          break;
        }
      case hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady:
        {
          hxc_l_gc_roots[6] = hxc_l_g.hxc_tag == hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady ? (const void *)hxc_l_g.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference : NULL;
          if (hxc_l_g.hxc_tag != hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady)
          {
            abort();
          }
          struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_tmp_enum_payload_project_n8 = hxc_l_g.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference;
          hxc_l_gc_roots[7] = (const void *)hxc_l_tmp_enum_payload_project_n8;
          struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_reference = hxc_l_tmp_enum_payload_project_n8;
          struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_tmp_load_result_n9 = hxc_l_reference;
          hxc_l_gc_roots[8] = (const void *)hxc_l_tmp_load_result_n9;
          if (hxc_l_tmp_load_result_n9 == NULL)
          {
            abort();
          }
          hxc_string hxc_l_tmp_class_field_load_result_n10 = (*hxc_l_tmp_load_result_n9).hxc_id;
          struct hxc_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition *hxc_l_tmp_instance_call_result_n11 = hxc_caxecraft_content_RuntimeContentRegistry_findAquatic(hxc_l_self, hxc_l_tmp_class_field_load_result_n10);
          hxc_l_gc_roots[9] = (const void *)hxc_l_tmp_instance_call_result_n11;
          struct hxc_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition *hxc_l_definition = hxc_l_tmp_instance_call_result_n11;
          hxc_l_gc_roots[10] = (const void *)hxc_l_definition;
          hxc_l_gc_roots[11] = (const void *)NULL;
          if (hxc_l_definition != NULL)
          {
            struct hxc_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition *hxc_l_tmp_load_result_n13 = hxc_l_definition;
            hxc_l_gc_roots[12] = (const void *)hxc_l_tmp_load_result_n13;
            if (hxc_l_tmp_load_result_n13 == NULL)
            {
              abort();
            }
            struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_instance_call_result_n14 = hxc_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition_profile(hxc_l_tmp_load_result_n13);
            if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
            {
              abort();
            }
            return hxc_l_tmp_instance_call_result_n14;
          }
          break;
        }
      default:
        {
          abort();
        }
    }
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_instance_call_result_n15 = hxc_caxecraft_content_RuntimeContentRegistry_defaultAquaticProfile(hxc_l_self);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_instance_call_result_n15;
}

int32_t hxc_caxecraft_content_RuntimeContentRegistry_itemPlacementBlockStorageCode(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, int32_t hxc_l_code)
{
  const void *hxc_l_gc_roots[9] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 9, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_reference = { 0 };
  bool hxc_l_tmp_short_circuit_result_n2 = hxc_l_code >= 0;
  if (hxc_l_code >= 0)
  {
    if (hxc_l_self == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_items;
    hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
    int32_t hxc_l_tmp_array_length_result_n1;
    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n0, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n2 = hxc_l_code < hxc_l_tmp_array_length_result_n1;
  }
  if (!hxc_l_tmp_short_circuit_result_n2)
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
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n3 = (*hxc_l_self).hxc_items;
  hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_class_field_load_result_n3;
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_tmp_array_get_result_n4;
  if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n3, (size_t)hxc_l_code, &hxc_l_tmp_array_get_result_n4) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_array_get_result_n4;
  if (hxc_l_tmp_array_get_result_n4 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference hxc_l_tmp_class_field_load_result_n5 = (*hxc_l_tmp_array_get_result_n4).hxc_placementBlock;
  hxc_l_gc_roots[4] = hxc_l_tmp_class_field_load_result_n5.hxc_tag == hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady ? (const void *)hxc_l_tmp_class_field_load_result_n5.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference : NULL;
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference hxc_l_g = hxc_l_tmp_class_field_load_result_n5;
  hxc_l_gc_roots[5] = hxc_l_g.hxc_tag == hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady ? (const void *)hxc_l_g.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference : NULL;
  switch (hxc_l_g.hxc_tag) {
    case hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_NoRuntimeReference:
      {
        break;
      }
    case hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady:
      {
        hxc_l_gc_roots[6] = hxc_l_g.hxc_tag == hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady ? (const void *)hxc_l_g.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference : NULL;
        if (hxc_l_g.hxc_tag != hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady)
        {
          abort();
        }
        struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_tmp_enum_payload_project_n8 = hxc_l_g.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference;
        hxc_l_gc_roots[7] = (const void *)hxc_l_tmp_enum_payload_project_n8;
        hxc_l_reference = hxc_l_tmp_enum_payload_project_n8;
        struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_tmp_load_result_n9 = hxc_l_reference;
        hxc_l_gc_roots[8] = (const void *)hxc_l_tmp_load_result_n9;
        if (hxc_l_tmp_load_result_n9 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_class_field_load_result_n10 = (*hxc_l_tmp_load_result_n9).hxc_id;
        int32_t hxc_l_tmp_instance_call_result_n11 = hxc_caxecraft_content_RuntimeContentRegistry_blockStorageCode(hxc_l_self, hxc_l_tmp_class_field_load_result_n10);
        if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
        {
          abort();
        }
        return hxc_l_tmp_instance_call_result_n11;
      }
    default:
      {
        abort();
      }
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return -1;
}

struct hxc_caxecraft_content_RuntimePresentation *hxc_caxecraft_content_RuntimeContentRegistry_itemPresentation(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, int32_t hxc_l_code)
{
  const void *hxc_l_gc_roots[6] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 6, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_tmp_short_circuit_result_n2 = hxc_l_code >= 0;
  if (hxc_l_code >= 0)
  {
    if (hxc_l_self == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_items;
    hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
    int32_t hxc_l_tmp_array_length_result_n1;
    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n0, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n2 = hxc_l_code < hxc_l_tmp_array_length_result_n1;
  }
  if (!hxc_l_tmp_short_circuit_result_n2)
  {
    hxc_l_gc_roots[5] = (const void *)NULL;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n3 = (*hxc_l_self).hxc_items;
  hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_class_field_load_result_n3;
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_tmp_array_get_result_n4;
  if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n3, (size_t)hxc_l_code, &hxc_l_tmp_array_get_result_n4) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_array_get_result_n4;
  if (hxc_l_tmp_array_get_result_n4 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_class_field_load_result_n5 = (*hxc_l_tmp_array_get_result_n4).hxc_presentation;
  hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_class_field_load_result_n5;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_class_field_load_result_n5;
}

bool hxc_caxecraft_content_RuntimeContentRegistry_itemProvidesAquaticProfile(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, int32_t hxc_l_code)
{
  const void *hxc_l_gc_roots[5] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 5, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_tmp_short_circuit_result_n2 = hxc_l_code >= 0;
  if (hxc_l_code >= 0)
  {
    if (hxc_l_self == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_items;
    hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
    int32_t hxc_l_tmp_array_length_result_n1;
    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n0, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n2 = hxc_l_code < hxc_l_tmp_array_length_result_n1;
  }
  if (!hxc_l_tmp_short_circuit_result_n2)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return false;
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n3 = (*hxc_l_self).hxc_items;
  hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_class_field_load_result_n3;
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_tmp_array_get_result_n4;
  if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n3, (size_t)hxc_l_code, &hxc_l_tmp_array_get_result_n4) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_array_get_result_n4;
  if (hxc_l_tmp_array_get_result_n4 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference hxc_l_tmp_class_field_load_result_n5 = (*hxc_l_tmp_array_get_result_n4).hxc_aquaticProfile;
  hxc_l_gc_roots[4] = hxc_l_tmp_class_field_load_result_n5.hxc_tag == hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady ? (const void *)hxc_l_tmp_class_field_load_result_n5.hxc_payload.hxc_RuntimeReferenceReady.hxc_reference : NULL;
  switch (hxc_l_tmp_class_field_load_result_n5.hxc_tag) {
    case hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_NoRuntimeReference:
      {
        break;
      }
    case hxc_caxecraft_content_RuntimeContentPack_RuntimeOptionalReference_RuntimeReferenceReady:
      {
        if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
        {
          abort();
        }
        return true;
      }
    default:
      {
        abort();
      }
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return false;
}

int32_t hxc_caxecraft_content_RuntimeContentRegistry_itemStorageCode(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[4] = { (const void *)hxc_l_self, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 4, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g = 0;
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_items;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
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
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n5 = (*hxc_l_self).hxc_items;
    hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_class_field_load_result_n5;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_tmp_array_get_result_n7;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n5, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_array_get_result_n7;
    if (hxc_l_tmp_array_get_result_n7 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n8 = (*hxc_l_tmp_array_get_result_n7).hxc_base.hxc_id;
    if (hxc_l_tmp_class_field_load_result_n8.data == NULL || hxc_l_id.data == NULL ? hxc_l_tmp_class_field_load_result_n8.data == hxc_l_id.data : hxc_l_tmp_class_field_load_result_n8.byte_length == hxc_l_id.byte_length && (hxc_l_tmp_class_field_load_result_n8.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n8.data, hxc_l_id.data, hxc_l_tmp_class_field_load_result_n8.byte_length) == 0))
    {
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_index;
    }
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return -1;
}

enum hxc_caxecraft_content_RuntimeItemUseProfile hxc_caxecraft_content_RuntimeContentRegistry_itemUseProfile(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, int32_t hxc_l_code)
{
  const void *hxc_l_gc_roots[4] = { (const void *)hxc_l_self, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 4, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_tmp_short_circuit_result_n2 = hxc_l_code >= 0;
  if (hxc_l_code >= 0)
  {
    if (hxc_l_self == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_items;
    hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
    int32_t hxc_l_tmp_array_length_result_n1;
    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n0, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n2 = hxc_l_code < hxc_l_tmp_array_length_result_n1;
  }
  if (!hxc_l_tmp_short_circuit_result_n2)
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_caxecraft_content_RuntimeItemUseProfile_NoItemUse;
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n3 = (*hxc_l_self).hxc_items;
  hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_class_field_load_result_n3;
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_tmp_array_get_result_n4;
  if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n3, (size_t)hxc_l_code, &hxc_l_tmp_array_get_result_n4) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_array_get_result_n4;
  if (hxc_l_tmp_array_get_result_n4 == NULL)
  {
    abort();
  }
  enum hxc_caxecraft_content_RuntimeItemUseProfile hxc_l_tmp_class_field_load_result_n5 = (*hxc_l_tmp_array_get_result_n4).hxc_useProfile;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_class_field_load_result_n5;
}

int32_t hxc_caxecraft_content_RuntimeContentRegistry_maximumItemQuantity(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[5] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 5, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeContentRegistry_findItem(hxc_l_self, hxc_l_id);
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_instance_call_result_n0;
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_item = hxc_l_tmp_instance_call_result_n0;
  hxc_l_gc_roots[2] = (const void *)hxc_l_item;
  hxc_l_gc_roots[3] = (const void *)NULL;
  if (!(hxc_l_item == NULL))
  {
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeItemDefinition *hxc_l_tmp_load_result_n2 = hxc_l_item;
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n2;
    if (hxc_l_tmp_load_result_n2 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n3 = (*hxc_l_tmp_load_result_n2).hxc_maximumStack;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_class_field_load_result_n3;
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return 0;
}

struct hxc_caxecraft_content_ActorPresentationResolution hxc_caxecraft_content_RuntimeContentRegistry_resolveActorPresentation(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[11] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 11, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeNpcDefinition *hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeContentRegistry_findNpc(hxc_l_self, hxc_l_id);
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_instance_call_result_n0;
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeNpcDefinition *hxc_l_npc = hxc_l_tmp_instance_call_result_n0;
  hxc_l_gc_roots[2] = (const void *)hxc_l_npc;
  hxc_l_gc_roots[3] = (const void *)NULL;
  if (hxc_l_npc != NULL)
  {
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeNpcDefinition *hxc_l_tmp_load_result_n2 = hxc_l_npc;
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n2;
    if (hxc_l_tmp_load_result_n2 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_class_field_load_result_n3 = (*hxc_l_tmp_load_result_n2).hxc_presentation;
    hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_class_field_load_result_n3;
    if (hxc_l_tmp_class_field_load_result_n3 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n4 = (*hxc_l_tmp_class_field_load_result_n3).hxc_cellIndex;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_ActorPresentationResolution){ .hxc_tag = hxc_caxecraft_content_ActorPresentationResolution_ActorPresentationResolved, .hxc_payload.hxc_ActorPresentationResolved.hxc_cellIndex = hxc_l_tmp_class_field_load_result_n4 };
  }
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_tmp_instance_call_result_n6 = hxc_caxecraft_content_RuntimeContentRegistry_findEnemy(hxc_l_self, hxc_l_id);
  hxc_l_gc_roots[6] = (const void *)hxc_l_tmp_instance_call_result_n6;
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_enemy = hxc_l_tmp_instance_call_result_n6;
  hxc_l_gc_roots[7] = (const void *)hxc_l_enemy;
  hxc_l_gc_roots[8] = (const void *)NULL;
  if (!(hxc_l_enemy == NULL))
  {
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_tmp_load_result_n9 = hxc_l_enemy;
    hxc_l_gc_roots[9] = (const void *)hxc_l_tmp_load_result_n9;
    if (hxc_l_tmp_load_result_n9 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_class_field_load_result_n10 = (*hxc_l_tmp_load_result_n9).hxc_presentation;
    hxc_l_gc_roots[10] = (const void *)hxc_l_tmp_class_field_load_result_n10;
    if (hxc_l_tmp_class_field_load_result_n10 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n11 = (*hxc_l_tmp_class_field_load_result_n10).hxc_cellIndex;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_ActorPresentationResolution){ .hxc_tag = hxc_caxecraft_content_ActorPresentationResolution_ActorPresentationResolved, .hxc_payload.hxc_ActorPresentationResolved.hxc_cellIndex = hxc_l_tmp_class_field_load_result_n11 };
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_content_ActorPresentationResolution){ .hxc_tag = hxc_caxecraft_content_ActorPresentationResolution_UnknownActorPresentation };
}

struct hxc_caxecraft_content_ActorContentResolution hxc_caxecraft_content_RuntimeContentRegistry_resolveEnemy(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[15] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 15, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_record_field_drop_owner_n3 = { 0 };
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeContentRegistry_findEnemy(hxc_l_self, hxc_l_id);
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_instance_call_result_n0;
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_enemy = hxc_l_tmp_instance_call_result_n0;
  hxc_l_gc_roots[2] = (const void *)hxc_l_enemy;
  hxc_l_gc_roots[3] = (const void *)NULL;
  if (hxc_l_enemy != NULL)
  {
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_tmp_load_result_n2 = hxc_l_enemy;
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n2;
    if (hxc_l_tmp_load_result_n2 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n3 = (*hxc_l_tmp_load_result_n2).hxc_maximumHealth;
    struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_instance_call_result_n4 = hxc_caxecraft_content_RuntimeContentRegistry_defaultAquaticProfile(hxc_l_self);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_tmp_load_result_n5 = hxc_l_enemy;
    hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_load_result_n5;
    if (hxc_l_tmp_load_result_n5 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n6 = (*hxc_l_tmp_load_result_n5).hxc_noticeRadiusMilli;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_tmp_load_result_n7 = hxc_l_enemy;
    hxc_l_gc_roots[6] = (const void *)hxc_l_tmp_load_result_n7;
    if (hxc_l_tmp_load_result_n7 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n8 = (*hxc_l_tmp_load_result_n7).hxc_strikeRadiusMilli;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_tmp_load_result_n9 = hxc_l_enemy;
    hxc_l_gc_roots[7] = (const void *)hxc_l_tmp_load_result_n9;
    if (hxc_l_tmp_load_result_n9 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n10 = (*hxc_l_tmp_load_result_n9).hxc_attackRadiusMilli;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_tmp_load_result_n11 = hxc_l_enemy;
    hxc_l_gc_roots[8] = (const void *)hxc_l_tmp_load_result_n11;
    if (hxc_l_tmp_load_result_n11 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n12 = (*hxc_l_tmp_load_result_n11).hxc_windupTicks;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_tmp_load_result_n13 = hxc_l_enemy;
    hxc_l_gc_roots[9] = (const void *)hxc_l_tmp_load_result_n13;
    if (hxc_l_tmp_load_result_n13 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n14 = (*hxc_l_tmp_load_result_n13).hxc_recoveryTicks;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_tmp_load_result_n15 = hxc_l_enemy;
    hxc_l_gc_roots[10] = (const void *)hxc_l_tmp_load_result_n15;
    if (hxc_l_tmp_load_result_n15 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n16 = (*hxc_l_tmp_load_result_n15).hxc_stepMilli;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_tmp_load_result_n17 = hxc_l_enemy;
    hxc_l_gc_roots[11] = (const void *)hxc_l_tmp_load_result_n17;
    if (hxc_l_tmp_load_result_n17 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeReference *hxc_l_tmp_class_field_load_result_n18 = (*hxc_l_tmp_load_result_n17).hxc_drop;
    hxc_l_gc_roots[12] = (const void *)hxc_l_tmp_class_field_load_result_n18;
    if (hxc_l_tmp_class_field_load_result_n18 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n19 = (*hxc_l_tmp_class_field_load_result_n18).hxc_id;
    hxc_l_tmp_record_field_drop_owner_n3 = hxc_l_tmp_class_field_load_result_n19;
    if (hxc_string_retain(hxc_l_tmp_record_field_drop_owner_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_ActorContentResolution){ .hxc_tag = hxc_caxecraft_content_ActorContentResolution_ActorContentResolved, .hxc_payload.hxc_ActorContentResolved.hxc_profile = (struct hxc_caxecraft_content_ActorMechanicsProfile){ .hxc_aquaticProfile = hxc_l_tmp_instance_call_result_n4, .hxc_controller = (struct hxc_caxecraft_domain_ActorControllerProfile){ .hxc_tag = hxc_caxecraft_domain_ActorControllerProfile_WanderChaseMelee, .hxc_payload.hxc_WanderChaseMelee.hxc_profile = (struct hxc_caxecraft_domain_WanderChaseMeleeProfile){ .hxc_attackRadiusMilli = hxc_l_tmp_class_field_load_result_n10, .hxc_drop = hxc_l_tmp_record_field_drop_owner_n3, .hxc_noticeRadiusMilli = hxc_l_tmp_class_field_load_result_n6, .hxc_recoveryTicks = hxc_l_tmp_class_field_load_result_n14, .hxc_stepMilli = hxc_l_tmp_class_field_load_result_n16, .hxc_strikeRadiusMilli = hxc_l_tmp_class_field_load_result_n8, .hxc_windupTicks = hxc_l_tmp_class_field_load_result_n12 } }, .hxc_maximumHealth = hxc_l_tmp_class_field_load_result_n3 } };
  }
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeNpcDefinition *hxc_l_tmp_instance_call_result_n25 = hxc_caxecraft_content_RuntimeContentRegistry_findNpc(hxc_l_self, hxc_l_id);
  hxc_l_gc_roots[13] = (const void *)hxc_l_tmp_instance_call_result_n25;
  hxc_l_gc_roots[14] = (const void *)NULL;
  if (!(hxc_l_tmp_instance_call_result_n25 != NULL))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_ActorContentResolution){ .hxc_tag = hxc_caxecraft_content_ActorContentResolution_UnknownActorContent };
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_content_ActorContentResolution){ .hxc_tag = hxc_caxecraft_content_ActorContentResolution_WrongActorContentKind, .hxc_payload.hxc_WrongActorContentKind.hxc_actual = hxc_caxecraft_content_ActorContentKind_NpcContent };
}

struct hxc_caxecraft_content_FluidContentResolution hxc_caxecraft_content_RuntimeContentRegistry_resolveFluid(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[6] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 6, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeFluidDefinition *hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeContentRegistry_findFluid(hxc_l_self, hxc_l_id);
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_instance_call_result_n0;
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeFluidDefinition *hxc_l_fluid = hxc_l_tmp_instance_call_result_n0;
  hxc_l_gc_roots[2] = (const void *)hxc_l_fluid;
  hxc_l_gc_roots[3] = (const void *)NULL;
  if (!(hxc_l_fluid == NULL))
  {
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeFluidDefinition *hxc_l_tmp_load_result_n4 = hxc_l_fluid;
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n4;
    if (hxc_l_tmp_load_result_n4 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimePresentation *hxc_l_tmp_class_field_load_result_n5 = (*hxc_l_tmp_load_result_n4).hxc_presentation;
    hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_class_field_load_result_n5;
    if (hxc_l_tmp_class_field_load_result_n5 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n6 = (*hxc_l_tmp_class_field_load_result_n5).hxc_cellIndex;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_FluidContentResolution){ .hxc_tag = hxc_caxecraft_content_FluidContentResolution_FluidContentResolved, .hxc_payload.hxc_FluidContentResolved.hxc_simulation = hxc_caxecraft_content_LevelFluidSimulation_BoundedWater, .hxc_payload.hxc_FluidContentResolved.hxc_presentationCell = hxc_l_tmp_class_field_load_result_n6 };
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_content_FluidContentResolution){ .hxc_tag = hxc_caxecraft_content_FluidContentResolution_UnknownFluidContent };
}

struct hxc_caxecraft_content_ItemContentResolution hxc_caxecraft_content_RuntimeContentRegistry_resolveItem(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
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
  int32_t hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeContentRegistry_itemStorageCode(hxc_l_self, hxc_l_id);
  int32_t hxc_l_code = hxc_l_tmp_instance_call_result_n0;
  if (!(hxc_l_code < 0))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_ItemContentResolution){ .hxc_tag = hxc_caxecraft_content_ItemContentResolution_ItemContentResolved, .hxc_payload.hxc_ItemContentResolved.hxc_code = hxc_l_code };
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_content_ItemContentResolution){ .hxc_tag = hxc_caxecraft_content_ItemContentResolution_UnknownItemContent };
}

struct hxc_caxecraft_content_ActorContentResolution hxc_caxecraft_content_RuntimeContentRegistry_resolveNpc(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[8] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 8, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeNpcDefinition *hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeContentRegistry_findNpc(hxc_l_self, hxc_l_id);
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_instance_call_result_n0;
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeNpcDefinition *hxc_l_npc = hxc_l_tmp_instance_call_result_n0;
  hxc_l_gc_roots[2] = (const void *)hxc_l_npc;
  hxc_l_gc_roots[3] = (const void *)NULL;
  if (hxc_l_npc != NULL)
  {
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeNpcDefinition *hxc_l_tmp_load_result_n2 = hxc_l_npc;
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n2;
    if (hxc_l_tmp_load_result_n2 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n3 = (*hxc_l_tmp_load_result_n2).hxc_maximumHealth;
    struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_instance_call_result_n4 = hxc_caxecraft_content_RuntimeContentRegistry_defaultAquaticProfile(hxc_l_self);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeNpcDefinition *hxc_l_tmp_load_result_n5 = hxc_l_npc;
    hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_load_result_n5;
    if (hxc_l_tmp_load_result_n5 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n6 = (*hxc_l_tmp_load_result_n5).hxc_interactionRadiusMilli;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_ActorContentResolution){ .hxc_tag = hxc_caxecraft_content_ActorContentResolution_ActorContentResolved, .hxc_payload.hxc_ActorContentResolved.hxc_profile = (struct hxc_caxecraft_content_ActorMechanicsProfile){ .hxc_aquaticProfile = hxc_l_tmp_instance_call_result_n4, .hxc_controller = (struct hxc_caxecraft_domain_ActorControllerProfile){ .hxc_tag = hxc_caxecraft_domain_ActorControllerProfile_StationaryDialogue, .hxc_payload.hxc_StationaryDialogue.hxc_interactionRadiusMilli = hxc_l_tmp_class_field_load_result_n6 }, .hxc_maximumHealth = hxc_l_tmp_class_field_load_result_n3 } };
  }
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeEnemyDefinition *hxc_l_tmp_instance_call_result_n10 = hxc_caxecraft_content_RuntimeContentRegistry_findEnemy(hxc_l_self, hxc_l_id);
  hxc_l_gc_roots[6] = (const void *)hxc_l_tmp_instance_call_result_n10;
  hxc_l_gc_roots[7] = (const void *)NULL;
  if (!(hxc_l_tmp_instance_call_result_n10 != NULL))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_ActorContentResolution){ .hxc_tag = hxc_caxecraft_content_ActorContentResolution_UnknownActorContent };
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_content_ActorContentResolution){ .hxc_tag = hxc_caxecraft_content_ActorContentResolution_WrongActorContentKind, .hxc_payload.hxc_WrongActorContentKind.hxc_actual = hxc_caxecraft_content_ActorContentKind_EnemyContent };
}

struct hxc_caxecraft_content_TerrainContentResolution hxc_caxecraft_content_RuntimeContentRegistry_resolveTerrain(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[5] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 5, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeContentRegistry_findBlock(hxc_l_self, hxc_l_id);
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_instance_call_result_n0;
  struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_block = hxc_l_tmp_instance_call_result_n0;
  hxc_l_gc_roots[2] = (const void *)hxc_l_block;
  hxc_l_gc_roots[3] = (const void *)NULL;
  if (!(hxc_l_block == NULL))
  {
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeBlockDefinition *hxc_l_tmp_load_result_n3 = hxc_l_block;
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n3;
    if (hxc_l_tmp_load_result_n3 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n4 = (*hxc_l_tmp_load_result_n3).hxc_storageCode;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_TerrainContentResolution){ .hxc_tag = hxc_caxecraft_content_TerrainContentResolution_TerrainContentResolved, .hxc_payload.hxc_TerrainContentResolved.hxc_code = hxc_l_tmp_class_field_load_result_n4 };
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_content_TerrainContentResolution){ .hxc_tag = hxc_caxecraft_content_TerrainContentResolution_UnknownTerrainContent };
}

bool hxc_caxecraft_content_RuntimeContentRegistry_supportsFeature(struct hxc_caxecraft_content_RuntimeContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[6] = { (const void *)hxc_l_self, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 6, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g = 0;
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_features;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  struct hxc_array_ref *hxc_l_g1 = hxc_l_tmp_class_field_load_result_n0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_g;
    hxc_l_gc_roots[2] = (const void *)hxc_l_g1;
    int32_t hxc_l_tmp_array_length_result_n3;
    if (hxc_array_ref_length(hxc_l_g1, &hxc_l_tmp_array_length_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n1 < hxc_l_tmp_array_length_result_n3))
    {
      break;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n4 = hxc_l_g1;
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_load_result_n4;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeLocatedId *hxc_l_tmp_array_get_result_n6;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n4, (size_t)hxc_l_g, &hxc_l_tmp_array_get_result_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_array_get_result_n6;
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeLocatedId *hxc_l_feature = hxc_l_tmp_array_get_result_n6;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_g, 1);
    struct hxc_caxecraft_content_RuntimeContentPack_RuntimeLocatedId *hxc_l_tmp_load_result_n8 = hxc_l_feature;
    hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_load_result_n8;
    if (hxc_l_tmp_load_result_n8 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n9 = (*hxc_l_tmp_load_result_n8).hxc_id;
    if (hxc_l_tmp_class_field_load_result_n9.data == NULL || hxc_l_id.data == NULL ? hxc_l_tmp_class_field_load_result_n9.data == hxc_l_id.data : hxc_l_tmp_class_field_load_result_n9.byte_length == hxc_l_id.byte_length && (hxc_l_tmp_class_field_load_result_n9.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n9.data, hxc_l_id.data, hxc_l_tmp_class_field_load_result_n9.byte_length) == 0))
    {
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return true;
    }
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return false;
}

struct hxc_caxecraft_domain_AquaticProfile hxc_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition_profile(struct hxc_caxecraft_content_RuntimeContentPack_RuntimeAquaticDefinition *hxc_l_self)
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
  int32_t hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_maximumBreathTicks;
  int32_t hxc_l_tmp_class_field_load_result_n1 = (*hxc_l_self).hxc_breathRecoveryPerTick;
  int32_t hxc_l_tmp_class_field_load_result_n2 = (*hxc_l_self).hxc_horizontalControlMilli;
  int32_t hxc_l_tmp_class_field_load_result_n3 = (*hxc_l_self).hxc_ascentAccelerationMilli;
  int32_t hxc_l_tmp_class_field_load_result_n4 = (*hxc_l_self).hxc_descentAccelerationMilli;
  int32_t hxc_l_tmp_class_field_load_result_n5 = (*hxc_l_self).hxc_buoyancyAccelerationMilli;
  int32_t hxc_l_tmp_class_field_load_result_n6 = (*hxc_l_self).hxc_dragPerTickMilli;
  int32_t hxc_l_tmp_class_field_load_result_n7 = (*hxc_l_self).hxc_drowningIntervalTicks;
  bool hxc_l_tmp_class_field_load_result_n8 = (*hxc_l_self).hxc_underwaterMining;
  bool hxc_l_tmp_class_field_load_result_n9 = (*hxc_l_self).hxc_coldProtection;
  struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_call_result_n10 = hxc_caxecraft_domain_Aquatics_profile(hxc_l_tmp_class_field_load_result_n0, hxc_l_tmp_class_field_load_result_n1, hxc_f64_divide_zero_safe((double)hxc_l_tmp_class_field_load_result_n2, 1000.0), hxc_f64_divide_zero_safe((double)hxc_l_tmp_class_field_load_result_n3, 1000.0), hxc_f64_divide_zero_safe((double)hxc_l_tmp_class_field_load_result_n4, 1000.0), hxc_f64_divide_zero_safe((double)hxc_l_tmp_class_field_load_result_n5, 1000.0), hxc_f64_divide_zero_safe((double)hxc_l_tmp_class_field_load_result_n6, 1000.0), hxc_l_tmp_class_field_load_result_n7, hxc_l_tmp_class_field_load_result_n8, hxc_l_tmp_class_field_load_result_n9);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_call_result_n10;
}
