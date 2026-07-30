#include "hxc/program.h"

int32_t hxc_caxecraft_content_BaseContentPack_aquaticAscentAccelerationMilli(int32_t hxc_l_value)
{
  switch (hxc_l_value) {
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

int32_t hxc_caxecraft_content_BaseContentPack_aquaticBreathRecoveryPerTick(int32_t hxc_l_value)
{
  switch (hxc_l_value) {
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

int32_t hxc_caxecraft_content_BaseContentPack_aquaticBuoyancyAccelerationMilli(int32_t hxc_l_value)
{
  switch (hxc_l_value) {
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

bool hxc_caxecraft_content_BaseContentPack_aquaticColdProtection(int32_t hxc_l_value)
{
  switch (hxc_l_value) {
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

int32_t hxc_caxecraft_content_BaseContentPack_aquaticDescentAccelerationMilli(int32_t hxc_l_value)
{
  switch (hxc_l_value) {
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

int32_t hxc_caxecraft_content_BaseContentPack_aquaticDragPerTickMilli(int32_t hxc_l_value)
{
  switch (hxc_l_value) {
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

int32_t hxc_caxecraft_content_BaseContentPack_aquaticDrowningIntervalTicks(int32_t hxc_l_value)
{
  switch (hxc_l_value) {
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

int32_t hxc_caxecraft_content_BaseContentPack_aquaticHorizontalControlMilli(int32_t hxc_l_value)
{
  switch (hxc_l_value) {
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

int32_t hxc_caxecraft_content_BaseContentPack_aquaticMaximumBreathTicks(int32_t hxc_l_value)
{
  switch (hxc_l_value) {
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

struct hxc_caxecraft_domain_AquaticProfile hxc_caxecraft_content_BaseContentPack_aquaticProfile(int32_t hxc_l_value)
{
  int32_t hxc_l_tmp_call_result_n0 = hxc_caxecraft_content_BaseContentPack_aquaticMaximumBreathTicks(hxc_l_value);
  int32_t hxc_l_tmp_call_result_n1 = hxc_caxecraft_content_BaseContentPack_aquaticBreathRecoveryPerTick(hxc_l_value);
  int32_t hxc_l_tmp_call_result_n2 = hxc_caxecraft_content_BaseContentPack_aquaticHorizontalControlMilli(hxc_l_value);
  int32_t hxc_l_tmp_call_result_n3 = hxc_caxecraft_content_BaseContentPack_aquaticAscentAccelerationMilli(hxc_l_value);
  int32_t hxc_l_tmp_call_result_n4 = hxc_caxecraft_content_BaseContentPack_aquaticDescentAccelerationMilli(hxc_l_value);
  int32_t hxc_l_tmp_call_result_n5 = hxc_caxecraft_content_BaseContentPack_aquaticBuoyancyAccelerationMilli(hxc_l_value);
  int32_t hxc_l_tmp_call_result_n6 = hxc_caxecraft_content_BaseContentPack_aquaticDragPerTickMilli(hxc_l_value);
  int32_t hxc_l_tmp_call_result_n7 = hxc_caxecraft_content_BaseContentPack_aquaticDrowningIntervalTicks(hxc_l_value);
  bool hxc_l_tmp_call_result_n8 = hxc_caxecraft_content_BaseContentPack_aquaticUnderwaterMining(hxc_l_value);
  bool hxc_l_tmp_call_result_n9 = hxc_caxecraft_content_BaseContentPack_aquaticColdProtection(hxc_l_value);
  struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_call_result_n10 = hxc_caxecraft_domain_Aquatics_profile(hxc_l_tmp_call_result_n0, hxc_l_tmp_call_result_n1, hxc_f64_divide_zero_safe((double)hxc_l_tmp_call_result_n2, 1000.0), hxc_f64_divide_zero_safe((double)hxc_l_tmp_call_result_n3, 1000.0), hxc_f64_divide_zero_safe((double)hxc_l_tmp_call_result_n4, 1000.0), hxc_f64_divide_zero_safe((double)hxc_l_tmp_call_result_n5, 1000.0), hxc_f64_divide_zero_safe((double)hxc_l_tmp_call_result_n6, 1000.0), hxc_l_tmp_call_result_n7, hxc_l_tmp_call_result_n8, hxc_l_tmp_call_result_n9);
  return hxc_l_tmp_call_result_n10;
}

bool hxc_caxecraft_content_BaseContentPack_aquaticUnderwaterMining(int32_t hxc_l_value)
{
  switch (hxc_l_value) {
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

hxc_string hxc_caxecraft_content_BaseContentPack_blockId(int32_t hxc_l_value)
{
  hxc_string hxc_l_tmp_returned_string_owner_n10 = { 0 };
  hxc_string hxc_l_tmp_returned_string_owner_n2 = { 0 };
  hxc_string hxc_l_tmp_returned_string_owner_n3 = { 0 };
  hxc_string hxc_l_tmp_returned_string_owner_n4 = { 0 };
  hxc_string hxc_l_tmp_returned_string_owner_n5 = { 0 };
  hxc_string hxc_l_tmp_returned_string_owner_n6 = { 0 };
  hxc_string hxc_l_tmp_returned_string_owner_n7 = { 0 };
  hxc_string hxc_l_tmp_returned_string_owner_n8 = { 0 };
  hxc_string hxc_l_tmp_returned_string_owner_n9 = { 0 };
  switch (hxc_l_value) {
    case 0:
      {
        break;
      }
    case 1:
      {
        hxc_l_tmp_returned_string_owner_n2 = (hxc_string){ (const uint8_t *)"caxecraft:ash", 13, true, NULL };
        if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n2) != HXC_STATUS_OK)
        {
          abort();
        }
        return hxc_l_tmp_returned_string_owner_n2;
      }
    case 2:
      {
        hxc_l_tmp_returned_string_owner_n3 = (hxc_string){ (const uint8_t *)"caxecraft:bedrock", 17, true, NULL };
        if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n3) != HXC_STATUS_OK)
        {
          abort();
        }
        return hxc_l_tmp_returned_string_owner_n3;
      }
    case 3:
      {
        hxc_l_tmp_returned_string_owner_n4 = (hxc_string){ (const uint8_t *)"caxecraft:dirt", 14, true, NULL };
        if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n4) != HXC_STATUS_OK)
        {
          abort();
        }
        return hxc_l_tmp_returned_string_owner_n4;
      }
    case 4:
      {
        hxc_l_tmp_returned_string_owner_n5 = (hxc_string){ (const uint8_t *)"caxecraft:grass", 15, true, NULL };
        if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n5) != HXC_STATUS_OK)
        {
          abort();
        }
        return hxc_l_tmp_returned_string_owner_n5;
      }
    case 5:
      {
        hxc_l_tmp_returned_string_owner_n6 = (hxc_string){ (const uint8_t *)"caxecraft:leaves", 16, true, NULL };
        if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n6) != HXC_STATUS_OK)
        {
          abort();
        }
        return hxc_l_tmp_returned_string_owner_n6;
      }
    case 6:
      {
        hxc_l_tmp_returned_string_owner_n7 = (hxc_string){ (const uint8_t *)"caxecraft:sand", 14, true, NULL };
        if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n7) != HXC_STATUS_OK)
        {
          abort();
        }
        return hxc_l_tmp_returned_string_owner_n7;
      }
    case 7:
      {
        hxc_l_tmp_returned_string_owner_n8 = (hxc_string){ (const uint8_t *)"caxecraft:snow", 14, true, NULL };
        if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n8) != HXC_STATUS_OK)
        {
          abort();
        }
        return hxc_l_tmp_returned_string_owner_n8;
      }
    case 8:
      {
        hxc_l_tmp_returned_string_owner_n9 = (hxc_string){ (const uint8_t *)"caxecraft:stone", 15, true, NULL };
        if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n9) != HXC_STATUS_OK)
        {
          abort();
        }
        return hxc_l_tmp_returned_string_owner_n9;
      }
    case 9:
      {
        hxc_l_tmp_returned_string_owner_n10 = (hxc_string){ (const uint8_t *)"caxecraft:wood", 14, true, NULL };
        if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n10) != HXC_STATUS_OK)
        {
          abort();
        }
        return hxc_l_tmp_returned_string_owner_n10;
      }
    default:
      {
        abort();
      }
  }
  hxc_string hxc_l_tmp_returned_string_owner_n1 = (hxc_string){ (const uint8_t *)"caxecraft:air", 13, true, NULL };
  if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_returned_string_owner_n1;
}

int32_t hxc_caxecraft_content_BaseContentPack_dropQuantityById(hxc_string hxc_l_id)
{
  if (!(hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:mossling-berries", 26, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:mossling-berries", 26, true, NULL }.data, hxc_l_id.byte_length) == 0))))
  {
    return 0;
  }
  return 2;
}

int32_t hxc_caxecraft_content_BaseContentPack_itemAquaticProfile(int32_t hxc_l_value)
{
  switch (hxc_l_value) {
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

int32_t hxc_caxecraft_content_BaseContentPack_itemFromValidatedStorageCode(int32_t hxc_l_code)
{
  if (hxc_l_code == 0)
  {
    return 0;
  }
  if (hxc_l_code == 1)
  {
    return 1;
  }
  if (hxc_l_code == 2)
  {
    return 2;
  }
  if (hxc_l_code == 3)
  {
    return 3;
  }
  if (hxc_l_code == 4)
  {
    return 4;
  }
  if (hxc_l_code == 5)
  {
    return 5;
  }
  if (hxc_l_code == 6)
  {
    return 6;
  }
  if (hxc_l_code == 7)
  {
    return 7;
  }
  if (!(hxc_l_code == 8))
  {
    return 0;
  }
  return 8;
}

struct hxc_caxecraft_content_ContentPresentation hxc_caxecraft_content_BaseContentPack_itemIcon(int32_t hxc_l_value)
{
  switch (hxc_l_value) {
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

bool hxc_caxecraft_content_BaseContentPack_itemProvidesAquaticProfile(int32_t hxc_l_value)
{
  switch (hxc_l_value) {
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

int32_t hxc_caxecraft_content_BaseContentPack_itemUseProfile(int32_t hxc_l_value)
{
  switch (hxc_l_value) {
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

bool hxc_caxecraft_content_BaseContentRegistry_hasBlock(struct hxc_caxecraft_content_BaseContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:air", 13, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:air", 13, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:ash", 13, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:ash", 13, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:bedrock", 17, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:bedrock", 17, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:dirt", 14, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:dirt", 14, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:grass", 15, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:grass", 15, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:leaves", 16, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:leaves", 16, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:sand", 14, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:sand", 14, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:snow", 14, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:snow", 14, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:stone", 15, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:stone", 15, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (!(hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:wood", 14, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:wood", 14, true, NULL }.data, hxc_l_id.byte_length) == 0))))
  {
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
  return true;
}

bool hxc_caxecraft_content_BaseContentRegistry_hasEffect(struct hxc_caxecraft_content_BaseContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:berry-pickup", 22, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:berry-pickup", 22, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (!(hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:copper-strike", 23, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:copper-strike", 23, true, NULL }.data, hxc_l_id.byte_length) == 0))))
  {
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
  return true;
}

bool hxc_caxecraft_content_BaseContentRegistry_hasEntity(struct hxc_caxecraft_content_BaseContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (!(hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:mossling", 18, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:mossling", 18, true, NULL }.data, hxc_l_id.byte_length) == 0))))
  {
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
  return true;
}

bool hxc_caxecraft_content_BaseContentRegistry_hasFluid(struct hxc_caxecraft_content_BaseContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (!(hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:water", 15, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:water", 15, true, NULL }.data, hxc_l_id.byte_length) == 0))))
  {
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
  return true;
}

bool hxc_caxecraft_content_BaseContentRegistry_hasItem(struct hxc_caxecraft_content_BaseContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:berries", 17, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:berries", 17, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:bread", 15, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:bread", 15, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:copper-sword", 22, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:copper-sword", 22, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:dirt-block", 20, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:dirt-block", 20, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:grass-block", 21, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:grass-block", 21, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:haxeforge", 19, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:haxeforge", 19, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:lantern", 17, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:lantern", 17, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:stone-block", 21, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:stone-block", 21, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return true;
  }
  if (!(hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:tideweave-suit", 24, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:tideweave-suit", 24, true, NULL }.data, hxc_l_id.byte_length) == 0))))
  {
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
  return true;
}

bool hxc_caxecraft_content_BaseContentRegistry_hasNpc(struct hxc_caxecraft_content_BaseContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (!(hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:nia", 13, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:nia", 13, true, NULL }.data, hxc_l_id.byte_length) == 0))))
  {
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
  return true;
}

bool hxc_caxecraft_content_BaseContentRegistry_hasPrefab(struct hxc_caxecraft_content_BaseContentRegistry *hxc_l_self, hxc_string hxc_l_id)
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

bool hxc_caxecraft_content_BaseContentRegistry_hasSignal(struct hxc_caxecraft_content_BaseContentRegistry *hxc_l_self, hxc_string hxc_l_id)
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

bool hxc_caxecraft_content_BaseContentRegistry_hasState(struct hxc_caxecraft_content_BaseContentRegistry *hxc_l_self, hxc_string hxc_l_id)
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

bool hxc_caxecraft_content_BaseContentRegistry_hasStatefulObject(struct hxc_caxecraft_content_BaseContentRegistry *hxc_l_self, hxc_string hxc_l_id)
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

bool hxc_caxecraft_content_BaseContentRegistry_isAirBlock(struct hxc_caxecraft_content_BaseContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:air", 13, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:air", 13, true, NULL }.data, hxc_l_id.byte_length) == 0));
}

int32_t hxc_caxecraft_content_BaseContentRegistry_maximumItemQuantity(struct hxc_caxecraft_content_BaseContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:berries", 17, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:berries", 17, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return 64;
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:bread", 15, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:bread", 15, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return 64;
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:copper-sword", 22, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:copper-sword", 22, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return 1;
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:dirt-block", 20, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:dirt-block", 20, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return 64;
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:grass-block", 21, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:grass-block", 21, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return 64;
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:haxeforge", 19, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:haxeforge", 19, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return 1;
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:lantern", 17, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:lantern", 17, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return 64;
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:stone-block", 21, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:stone-block", 21, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return 64;
  }
  if (!(hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:tideweave-suit", 24, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:tideweave-suit", 24, true, NULL }.data, hxc_l_id.byte_length) == 0))))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return 0;
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return 1;
}

struct hxc_caxecraft_content_ActorContentResolution hxc_caxecraft_content_BaseContentRegistry_resolveEnemy(struct hxc_caxecraft_content_BaseContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_record_field_drop_owner_n2 = { 0 };
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:mossling", 18, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:mossling", 18, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_call_result_n1 = hxc_caxecraft_content_BaseContentPack_aquaticProfile(0);
    hxc_l_tmp_record_field_drop_owner_n2 = (hxc_string){ (const uint8_t *)"caxecraft:mossling-berries", 26, true, NULL };
    if (hxc_string_retain(hxc_l_tmp_record_field_drop_owner_n2) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_ActorContentResolution){ .hxc_tag = hxc_caxecraft_content_ActorContentResolution_ActorContentResolved, .hxc_payload.hxc_ActorContentResolved.hxc_profile = (struct hxc_caxecraft_content_ActorMechanicsProfile){ .hxc_aquaticProfile = hxc_l_tmp_call_result_n1, .hxc_controller = (struct hxc_caxecraft_domain_ActorControllerProfile){ .hxc_tag = hxc_caxecraft_domain_ActorControllerProfile_WanderChaseMelee, .hxc_payload.hxc_WanderChaseMelee.hxc_profile = (struct hxc_caxecraft_domain_WanderChaseMeleeProfile){ .hxc_attackRadiusMilli = 1400, .hxc_drop = hxc_l_tmp_record_field_drop_owner_n2, .hxc_noticeRadiusMilli = 6000, .hxc_recoveryTicks = 12, .hxc_stepMilli = 80, .hxc_strikeRadiusMilli = 3000, .hxc_windupTicks = 8 } }, .hxc_maximumHealth = 3 } };
  }
  if (!(hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:nia", 13, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:nia", 13, true, NULL }.data, hxc_l_id.byte_length) == 0))))
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

struct hxc_caxecraft_content_FluidContentResolution hxc_caxecraft_content_BaseContentRegistry_resolveFluid(struct hxc_caxecraft_content_BaseContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (!(hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:water", 15, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:water", 15, true, NULL }.data, hxc_l_id.byte_length) == 0))))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_FluidContentResolution){ .hxc_tag = hxc_caxecraft_content_FluidContentResolution_UnknownFluidContent };
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_content_FluidContentResolution){ .hxc_tag = hxc_caxecraft_content_FluidContentResolution_FluidContentResolved, .hxc_payload.hxc_FluidContentResolved.hxc_simulation = hxc_caxecraft_content_LevelFluidSimulation_BoundedWater, .hxc_payload.hxc_FluidContentResolved.hxc_presentationCell = 5 };
}

struct hxc_caxecraft_content_ItemContentResolution hxc_caxecraft_content_BaseContentRegistry_resolveItem(struct hxc_caxecraft_content_BaseContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:berries", 17, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:berries", 17, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_ItemContentResolution){ .hxc_tag = hxc_caxecraft_content_ItemContentResolution_ItemContentResolved, .hxc_payload.hxc_ItemContentResolved.hxc_code = 0 };
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:bread", 15, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:bread", 15, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_ItemContentResolution){ .hxc_tag = hxc_caxecraft_content_ItemContentResolution_ItemContentResolved, .hxc_payload.hxc_ItemContentResolved.hxc_code = 1 };
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:copper-sword", 22, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:copper-sword", 22, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_ItemContentResolution){ .hxc_tag = hxc_caxecraft_content_ItemContentResolution_ItemContentResolved, .hxc_payload.hxc_ItemContentResolved.hxc_code = 2 };
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:dirt-block", 20, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:dirt-block", 20, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_ItemContentResolution){ .hxc_tag = hxc_caxecraft_content_ItemContentResolution_ItemContentResolved, .hxc_payload.hxc_ItemContentResolved.hxc_code = 3 };
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:grass-block", 21, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:grass-block", 21, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_ItemContentResolution){ .hxc_tag = hxc_caxecraft_content_ItemContentResolution_ItemContentResolved, .hxc_payload.hxc_ItemContentResolved.hxc_code = 4 };
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:haxeforge", 19, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:haxeforge", 19, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_ItemContentResolution){ .hxc_tag = hxc_caxecraft_content_ItemContentResolution_ItemContentResolved, .hxc_payload.hxc_ItemContentResolved.hxc_code = 5 };
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:lantern", 17, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:lantern", 17, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_ItemContentResolution){ .hxc_tag = hxc_caxecraft_content_ItemContentResolution_ItemContentResolved, .hxc_payload.hxc_ItemContentResolved.hxc_code = 6 };
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:stone-block", 21, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:stone-block", 21, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_ItemContentResolution){ .hxc_tag = hxc_caxecraft_content_ItemContentResolution_ItemContentResolved, .hxc_payload.hxc_ItemContentResolved.hxc_code = 7 };
  }
  if (!(hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:tideweave-suit", 24, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:tideweave-suit", 24, true, NULL }.data, hxc_l_id.byte_length) == 0))))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_ItemContentResolution){ .hxc_tag = hxc_caxecraft_content_ItemContentResolution_UnknownItemContent };
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_content_ItemContentResolution){ .hxc_tag = hxc_caxecraft_content_ItemContentResolution_ItemContentResolved, .hxc_payload.hxc_ItemContentResolved.hxc_code = 8 };
}

struct hxc_caxecraft_content_ActorContentResolution hxc_caxecraft_content_BaseContentRegistry_resolveNpc(struct hxc_caxecraft_content_BaseContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:nia", 13, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:nia", 13, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    struct hxc_caxecraft_domain_AquaticProfile hxc_l_tmp_call_result_n1 = hxc_caxecraft_content_BaseContentPack_aquaticProfile(0);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_ActorContentResolution){ .hxc_tag = hxc_caxecraft_content_ActorContentResolution_ActorContentResolved, .hxc_payload.hxc_ActorContentResolved.hxc_profile = (struct hxc_caxecraft_content_ActorMechanicsProfile){ .hxc_aquaticProfile = hxc_l_tmp_call_result_n1, .hxc_controller = (struct hxc_caxecraft_domain_ActorControllerProfile){ .hxc_tag = hxc_caxecraft_domain_ActorControllerProfile_StationaryDialogue, .hxc_payload.hxc_StationaryDialogue.hxc_interactionRadiusMilli = 3500 }, .hxc_maximumHealth = 6 } };
  }
  if (!(hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:mossling", 18, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:mossling", 18, true, NULL }.data, hxc_l_id.byte_length) == 0))))
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

struct hxc_caxecraft_content_TerrainContentResolution hxc_caxecraft_content_BaseContentRegistry_resolveTerrain(struct hxc_caxecraft_content_BaseContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:air", 13, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:air", 13, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_TerrainContentResolution){ .hxc_tag = hxc_caxecraft_content_TerrainContentResolution_TerrainContentResolved, .hxc_payload.hxc_TerrainContentResolved.hxc_code = 0 };
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:ash", 13, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:ash", 13, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_TerrainContentResolution){ .hxc_tag = hxc_caxecraft_content_TerrainContentResolution_TerrainContentResolved, .hxc_payload.hxc_TerrainContentResolved.hxc_code = 9 };
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:bedrock", 17, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:bedrock", 17, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_TerrainContentResolution){ .hxc_tag = hxc_caxecraft_content_TerrainContentResolution_TerrainContentResolved, .hxc_payload.hxc_TerrainContentResolved.hxc_code = 4 };
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:dirt", 14, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:dirt", 14, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_TerrainContentResolution){ .hxc_tag = hxc_caxecraft_content_TerrainContentResolution_TerrainContentResolved, .hxc_payload.hxc_TerrainContentResolved.hxc_code = 2 };
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:grass", 15, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:grass", 15, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_TerrainContentResolution){ .hxc_tag = hxc_caxecraft_content_TerrainContentResolution_TerrainContentResolved, .hxc_payload.hxc_TerrainContentResolved.hxc_code = 1 };
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:leaves", 16, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:leaves", 16, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_TerrainContentResolution){ .hxc_tag = hxc_caxecraft_content_TerrainContentResolution_TerrainContentResolved, .hxc_payload.hxc_TerrainContentResolved.hxc_code = 7 };
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:sand", 14, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:sand", 14, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_TerrainContentResolution){ .hxc_tag = hxc_caxecraft_content_TerrainContentResolution_TerrainContentResolved, .hxc_payload.hxc_TerrainContentResolved.hxc_code = 5 };
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:snow", 14, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:snow", 14, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_TerrainContentResolution){ .hxc_tag = hxc_caxecraft_content_TerrainContentResolution_TerrainContentResolved, .hxc_payload.hxc_TerrainContentResolved.hxc_code = 8 };
  }
  if (hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:stone", 15, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:stone", 15, true, NULL }.data, hxc_l_id.byte_length) == 0)))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_TerrainContentResolution){ .hxc_tag = hxc_caxecraft_content_TerrainContentResolution_TerrainContentResolved, .hxc_payload.hxc_TerrainContentResolved.hxc_code = 3 };
  }
  if (!(hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:wood", 14, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:wood", 14, true, NULL }.data, hxc_l_id.byte_length) == 0))))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_content_TerrainContentResolution){ .hxc_tag = hxc_caxecraft_content_TerrainContentResolution_UnknownTerrainContent };
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_content_TerrainContentResolution){ .hxc_tag = hxc_caxecraft_content_TerrainContentResolution_TerrainContentResolved, .hxc_payload.hxc_TerrainContentResolved.hxc_code = 6 };
}

bool hxc_caxecraft_content_BaseContentRegistry_supportsFeature(struct hxc_caxecraft_content_BaseContentRegistry *hxc_l_self, hxc_string hxc_l_id)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_id.data != NULL && (hxc_l_id.byte_length == (hxc_string){ (const uint8_t *)"caxecraft:core", 14, true, NULL }.byte_length && (hxc_l_id.byte_length == 0 || memcmp(hxc_l_id.data, (hxc_string){ (const uint8_t *)"caxecraft:core", 14, true, NULL }.data, hxc_l_id.byte_length) == 0));
}
