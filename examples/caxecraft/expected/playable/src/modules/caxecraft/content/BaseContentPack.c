#include "hxc/program.h"

int32_t hxc_caxecraft_content_BaseContentPack_aquaticAscentAccelerationMilli(int32_t hxc_value)
{
  switch (hxc_value) {
    case 0:
      {
        break;
      }
    case 1:
      {
        return 18000;
      }
    default:
      {
        abort();
      }
  }
  return 14000;
}

int32_t hxc_caxecraft_content_BaseContentPack_aquaticBreathRecoveryPerTick(int32_t hxc_value)
{
  switch (hxc_value) {
    case 0:
      {
        break;
      }
    case 1:
      {
        return 12;
      }
    default:
      {
        abort();
      }
  }
  return 4;
}

int32_t hxc_caxecraft_content_BaseContentPack_aquaticBuoyancyAccelerationMilli(int32_t hxc_value)
{
  switch (hxc_value) {
    case 0:
      {
        break;
      }
    case 1:
      {
        return 14000;
      }
    default:
      {
        abort();
      }
  }
  return 12000;
}

bool hxc_caxecraft_content_BaseContentPack_aquaticColdProtection(int32_t hxc_value)
{
  switch (hxc_value) {
    case 0:
      {
        break;
      }
    case 1:
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

int32_t hxc_caxecraft_content_BaseContentPack_aquaticDescentAccelerationMilli(int32_t hxc_value)
{
  switch (hxc_value) {
    case 0:
      {
        break;
      }
    case 1:
      {
        return 22000;
      }
    default:
      {
        abort();
      }
  }
  return 20000;
}

int32_t hxc_caxecraft_content_BaseContentPack_aquaticDragPerTickMilli(int32_t hxc_value)
{
  switch (hxc_value) {
    case 0:
      {
        break;
      }
    case 1:
      {
        return 100;
      }
    default:
      {
        abort();
      }
  }
  return 180;
}

int32_t hxc_caxecraft_content_BaseContentPack_aquaticDrowningIntervalTicks(int32_t hxc_value)
{
  switch (hxc_value) {
    case 0:
      {
        break;
      }
    case 1:
      {
        return 20;
      }
    default:
      {
        abort();
      }
  }
  return 20;
}

int32_t hxc_caxecraft_content_BaseContentPack_aquaticHorizontalControlMilli(int32_t hxc_value)
{
  switch (hxc_value) {
    case 0:
      {
        break;
      }
    case 1:
      {
        return 900;
      }
    default:
      {
        abort();
      }
  }
  return 350;
}

int32_t hxc_caxecraft_content_BaseContentPack_aquaticMaximumBreathTicks(int32_t hxc_value)
{
  switch (hxc_value) {
    case 0:
      {
        break;
      }
    case 1:
      {
        return 1200;
      }
    default:
      {
        abort();
      }
  }
  return 120;
}

struct hxc_caxecraft_domain_AquaticProfile hxc_caxecraft_content_BaseContentPack_aquaticProfile(int32_t hxc_value)
{
  int32_t hxc_tmp_call_result_n0 = hxc_caxecraft_content_BaseContentPack_aquaticMaximumBreathTicks(hxc_value);
  int32_t hxc_tmp_call_result_n1 = hxc_caxecraft_content_BaseContentPack_aquaticBreathRecoveryPerTick(hxc_value);
  int32_t hxc_tmp_call_result_n2 = hxc_caxecraft_content_BaseContentPack_aquaticHorizontalControlMilli(hxc_value);
  int32_t hxc_tmp_call_result_n3 = hxc_caxecraft_content_BaseContentPack_aquaticAscentAccelerationMilli(hxc_value);
  int32_t hxc_tmp_call_result_n4 = hxc_caxecraft_content_BaseContentPack_aquaticDescentAccelerationMilli(hxc_value);
  int32_t hxc_tmp_call_result_n5 = hxc_caxecraft_content_BaseContentPack_aquaticBuoyancyAccelerationMilli(hxc_value);
  int32_t hxc_tmp_call_result_n6 = hxc_caxecraft_content_BaseContentPack_aquaticDragPerTickMilli(hxc_value);
  int32_t hxc_tmp_call_result_n7 = hxc_caxecraft_content_BaseContentPack_aquaticDrowningIntervalTicks(hxc_value);
  bool hxc_tmp_call_result_n8 = hxc_caxecraft_content_BaseContentPack_aquaticUnderwaterMining(hxc_value);
  bool hxc_tmp_call_result_n9 = hxc_caxecraft_content_BaseContentPack_aquaticColdProtection(hxc_value);
  struct hxc_caxecraft_domain_AquaticProfile hxc_tmp_call_result_n10 = hxc_caxecraft_domain_Aquatics_profile(hxc_tmp_call_result_n0, hxc_tmp_call_result_n1, hxc_f64_divide_zero_safe((double)hxc_tmp_call_result_n2, 1000.0), hxc_f64_divide_zero_safe((double)hxc_tmp_call_result_n3, 1000.0), hxc_f64_divide_zero_safe((double)hxc_tmp_call_result_n4, 1000.0), hxc_f64_divide_zero_safe((double)hxc_tmp_call_result_n5, 1000.0), hxc_f64_divide_zero_safe((double)hxc_tmp_call_result_n6, 1000.0), hxc_tmp_call_result_n7, hxc_tmp_call_result_n8, hxc_tmp_call_result_n9);
  return hxc_tmp_call_result_n10;
}

bool hxc_caxecraft_content_BaseContentPack_aquaticUnderwaterMining(int32_t hxc_value)
{
  switch (hxc_value) {
    case 0:
      {
        break;
      }
    case 1:
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

int32_t hxc_caxecraft_content_BaseContentPack_itemAquaticProfile(int32_t hxc_value)
{
  switch (hxc_value) {
    case 0:
      {
        break;
      }
    case 1:
      {
        return 0;
      }
    case 2:
      {
        return 0;
      }
    case 3:
      {
        return 0;
      }
    case 4:
      {
        return 0;
      }
    case 5:
      {
        return 0;
      }
    case 6:
      {
        return 0;
      }
    case 7:
      {
        return 0;
      }
    case 8:
      {
        return 1;
      }
    default:
      {
        abort();
      }
  }
  return 0;
}

int32_t hxc_caxecraft_content_BaseContentPack_itemFromValidatedStorageCode(int32_t hxc_code)
{
  if (hxc_code == 0)
  {
    return 0;
  }
  if (hxc_code == 1)
  {
    return 1;
  }
  if (hxc_code == 2)
  {
    return 2;
  }
  if (hxc_code == 3)
  {
    return 3;
  }
  if (hxc_code == 4)
  {
    return 4;
  }
  if (hxc_code == 5)
  {
    return 5;
  }
  if (hxc_code == 6)
  {
    return 6;
  }
  if (hxc_code == 7)
  {
    return 7;
  }
  if (!(hxc_code == 8))
  {
    return 0;
  }
  return 8;
}

struct hxc_caxecraft_content_ContentPresentation hxc_caxecraft_content_BaseContentPack_itemIcon(int32_t hxc_value)
{
  switch (hxc_value) {
    case 0:
      {
        break;
      }
    case 1:
      {
        return (struct hxc_caxecraft_content_ContentPresentation){ .hxc_asset = 2, .hxc_cellIndex = 11 };
      }
    case 2:
      {
        return (struct hxc_caxecraft_content_ContentPresentation){ .hxc_asset = 2, .hxc_cellIndex = 7 };
      }
    case 3:
      {
        return (struct hxc_caxecraft_content_ContentPresentation){ .hxc_asset = 2, .hxc_cellIndex = 1 };
      }
    case 4:
      {
        return (struct hxc_caxecraft_content_ContentPresentation){ .hxc_asset = 2, .hxc_cellIndex = 0 };
      }
    case 5:
      {
        return (struct hxc_caxecraft_content_ContentPresentation){ .hxc_asset = 2, .hxc_cellIndex = 4 };
      }
    case 6:
      {
        return (struct hxc_caxecraft_content_ContentPresentation){ .hxc_asset = 2, .hxc_cellIndex = 13 };
      }
    case 7:
      {
        return (struct hxc_caxecraft_content_ContentPresentation){ .hxc_asset = 2, .hxc_cellIndex = 2 };
      }
    case 8:
      {
        return (struct hxc_caxecraft_content_ContentPresentation){ .hxc_asset = 0, .hxc_cellIndex = 0 };
      }
    default:
      {
        abort();
      }
  }
  return (struct hxc_caxecraft_content_ContentPresentation){ .hxc_asset = 2, .hxc_cellIndex = 10 };
}

bool hxc_caxecraft_content_BaseContentPack_itemProvidesAquaticProfile(int32_t hxc_value)
{
  switch (hxc_value) {
    case 0:
      {
        break;
      }
    case 1:
      {
        return false;
      }
    case 2:
      {
        return false;
      }
    case 3:
      {
        return false;
      }
    case 4:
      {
        return false;
      }
    case 5:
      {
        return false;
      }
    case 6:
      {
        return false;
      }
    case 7:
      {
        return false;
      }
    case 8:
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

int32_t hxc_caxecraft_content_BaseContentPack_itemUseProfile(int32_t hxc_value)
{
  switch (hxc_value) {
    case 0:
      {
        break;
      }
    case 1:
      {
        return 5;
      }
    case 2:
      {
        return 4;
      }
    case 3:
      {
        return 6;
      }
    case 4:
      {
        return 6;
      }
    case 5:
      {
        return 2;
      }
    case 6:
      {
        return 3;
      }
    case 7:
      {
        return 6;
      }
    case 8:
      {
        return 1;
      }
    default:
      {
        abort();
      }
  }
  return 0;
}

bool hxc_caxecraft_content_BaseContentRegistry_hasBlock(struct hxc_caxecraft_content_BaseContentRegistry *hxc_self, hxc_string hxc_id)
{
  const void *hxc_gc_roots[1] = { (const void *)hxc_self };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 1, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:air", 13, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:air", 13, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:ash", 13, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:ash", 13, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:bedrock", 17, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:bedrock", 17, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:dirt", 14, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:dirt", 14, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:grass", 15, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:grass", 15, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:leaves", 16, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:leaves", 16, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:sand", 14, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:sand", 14, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:snow", 14, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:snow", 14, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:stone", 15, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:stone", 15, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (!(hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:wood", 14, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:wood", 14, true, NULL }.data, hxc_id.byte_length) == 0))))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return false;
  }
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return true;
}

bool hxc_caxecraft_content_BaseContentRegistry_hasEffect(struct hxc_caxecraft_content_BaseContentRegistry *hxc_self, hxc_string hxc_id)
{
  const void *hxc_gc_roots[1] = { (const void *)hxc_self };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 1, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:berry-pickup", 22, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:berry-pickup", 22, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (!(hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:copper-strike", 23, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:copper-strike", 23, true, NULL }.data, hxc_id.byte_length) == 0))))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return false;
  }
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return true;
}

bool hxc_caxecraft_content_BaseContentRegistry_hasEntity(struct hxc_caxecraft_content_BaseContentRegistry *hxc_self, hxc_string hxc_id)
{
  const void *hxc_gc_roots[1] = { (const void *)hxc_self };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 1, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (!(hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:mossling", 18, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:mossling", 18, true, NULL }.data, hxc_id.byte_length) == 0))))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return false;
  }
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return true;
}

bool hxc_caxecraft_content_BaseContentRegistry_hasFluid(struct hxc_caxecraft_content_BaseContentRegistry *hxc_self, hxc_string hxc_id)
{
  const void *hxc_gc_roots[1] = { (const void *)hxc_self };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 1, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (!(hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:water", 15, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:water", 15, true, NULL }.data, hxc_id.byte_length) == 0))))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return false;
  }
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return true;
}

bool hxc_caxecraft_content_BaseContentRegistry_hasItem(struct hxc_caxecraft_content_BaseContentRegistry *hxc_self, hxc_string hxc_id)
{
  const void *hxc_gc_roots[1] = { (const void *)hxc_self };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 1, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:berries", 17, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:berries", 17, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:bread", 15, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:bread", 15, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:copper-sword", 22, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:copper-sword", 22, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:dirt-block", 20, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:dirt-block", 20, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:grass-block", 21, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:grass-block", 21, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:haxeforge", 19, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:haxeforge", 19, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:lantern", 17, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:lantern", 17, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:stone-block", 21, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:stone-block", 21, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (!(hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:tideweave-suit", 24, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:tideweave-suit", 24, true, NULL }.data, hxc_id.byte_length) == 0))))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return false;
  }
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return true;
}

bool hxc_caxecraft_content_BaseContentRegistry_hasNpc(struct hxc_caxecraft_content_BaseContentRegistry *hxc_self, hxc_string hxc_id)
{
  const void *hxc_gc_roots[1] = { (const void *)hxc_self };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 1, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (!(hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:nia", 13, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:nia", 13, true, NULL }.data, hxc_id.byte_length) == 0))))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return false;
  }
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return true;
}

bool hxc_caxecraft_content_BaseContentRegistry_hasPrefab(struct hxc_caxecraft_content_BaseContentRegistry *hxc_self, hxc_string hxc_id)
{
  const void *hxc_gc_roots[1] = { (const void *)hxc_self };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 1, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  (void)hxc_id;
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return false;
}

bool hxc_caxecraft_content_BaseContentRegistry_hasSignal(struct hxc_caxecraft_content_BaseContentRegistry *hxc_self, hxc_string hxc_id)
{
  const void *hxc_gc_roots[1] = { (const void *)hxc_self };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 1, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  (void)hxc_id;
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return false;
}

bool hxc_caxecraft_content_BaseContentRegistry_hasState(struct hxc_caxecraft_content_BaseContentRegistry *hxc_self, hxc_string hxc_id)
{
  const void *hxc_gc_roots[1] = { (const void *)hxc_self };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 1, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  (void)hxc_id;
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return false;
}

bool hxc_caxecraft_content_BaseContentRegistry_hasStatefulObject(struct hxc_caxecraft_content_BaseContentRegistry *hxc_self, hxc_string hxc_id)
{
  const void *hxc_gc_roots[1] = { (const void *)hxc_self };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 1, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  (void)hxc_id;
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return false;
}

bool hxc_caxecraft_content_BaseContentRegistry_isAirBlock(struct hxc_caxecraft_content_BaseContentRegistry *hxc_self, hxc_string hxc_id)
{
  const void *hxc_gc_roots[1] = { (const void *)hxc_self };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 1, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:air", 13, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:air", 13, true, NULL }.data, hxc_id.byte_length) == 0));
}

int32_t hxc_caxecraft_content_BaseContentRegistry_maximumItemQuantity(struct hxc_caxecraft_content_BaseContentRegistry *hxc_self, hxc_string hxc_id)
{
  const void *hxc_gc_roots[1] = { (const void *)hxc_self };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 1, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:berries", 17, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:berries", 17, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return 64;
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:bread", 15, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:bread", 15, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return 64;
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:copper-sword", 22, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:copper-sword", 22, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return 1;
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:dirt-block", 20, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:dirt-block", 20, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return 64;
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:grass-block", 21, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:grass-block", 21, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return 64;
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:haxeforge", 19, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:haxeforge", 19, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return 1;
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:lantern", 17, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:lantern", 17, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return 64;
  }
  if (hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:stone-block", 21, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:stone-block", 21, true, NULL }.data, hxc_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return 64;
  }
  if (!(hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:tideweave-suit", 24, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:tideweave-suit", 24, true, NULL }.data, hxc_id.byte_length) == 0))))
  {
    if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return 0;
  }
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return 1;
}

bool hxc_caxecraft_content_BaseContentRegistry_supportsFeature(struct hxc_caxecraft_content_BaseContentRegistry *hxc_self, hxc_string hxc_id)
{
  const void *hxc_gc_roots[1] = { (const void *)hxc_self };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 1, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_id.data != NULL && (hxc_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:core", 14, true, NULL }.byte_length && (hxc_id.byte_length == 0 || memcmp(hxc_id.data, (hxc_string){ (const uint8_t *)"caxecraft:core", 14, true, NULL }.data, hxc_id.byte_length) == 0));
}
