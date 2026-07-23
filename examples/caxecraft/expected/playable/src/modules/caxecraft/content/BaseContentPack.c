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
  if (!(hxc_code == 0))
  {
    if (!(hxc_code == 1))
    {
      if (!(hxc_code == 2))
      {
        if (!(hxc_code == 3))
        {
          if (!(hxc_code == 4))
          {
            if (!(hxc_code == 5))
            {
              if (!(hxc_code == 6))
              {
                if (!(hxc_code == 7))
                {
                  if (!(hxc_code == 8))
                  {
                    return 0;
                  }
                  return 8;
                }
                return 7;
              }
              return 6;
            }
            return 5;
          }
          return 4;
        }
        return 3;
      }
      return 2;
    }
    return 1;
  }
  return 0;
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
