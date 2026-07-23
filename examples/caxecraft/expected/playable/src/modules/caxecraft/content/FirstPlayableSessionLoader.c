#include "hxc/program.h"

struct hxc_caxecraft_content_FirstPlayableSessionLoad hxc_caxecraft_content_FirstPlayableSessionLoader_loadCandidate(struct hxc_caxecraft_domain_GameSession *hxc_session)
{
  int32_t hxc_tmp_call_result_n0 = hxc_caxecraft_content_FirstPlayableSessionLoader_validateFacts();
  int32_t hxc_presentationCell = hxc_tmp_call_result_n0;
  if (hxc_presentationCell < 0)
  {
    return (struct hxc_caxecraft_content_FirstPlayableSessionLoad){ .hxc_valid = false, .hxc_waterPresentationCell = -1 };
  }
  if (hxc_session == NULL)
  {
    abort();
  }
  hxc_caxecraft_domain_GameSession_resetEmptyWorld(hxc_session);
  bool hxc_tmp_call_result_n3 = hxc_caxecraft_content_FirstPlayableLevel_loadTerrain(hxc_session);
  if (!hxc_tmp_call_result_n3)
  {
    return (struct hxc_caxecraft_content_FirstPlayableSessionLoad){ .hxc_valid = false, .hxc_waterPresentationCell = -1 };
  }
  int32_t hxc_fluidIndex = 0;
  while (1)
  {
    if (!(hxc_fluidIndex < 2))
    {
      break;
    }
    int32_t hxc_tmp_call_result_n7 = hxc_caxecraft_content_FirstPlayableLevel_fluidX(hxc_fluidIndex);
    int32_t hxc_tmp_call_result_n9 = hxc_caxecraft_content_FirstPlayableLevel_fluidY(hxc_fluidIndex);
    int32_t hxc_tmp_call_result_n11 = hxc_caxecraft_content_FirstPlayableLevel_fluidZ(hxc_fluidIndex);
    struct hxc_caxecraft_domain_BlockCoord hxc_tmp_call_result_n12 = hxc_caxecraft_domain_World_coord(hxc_tmp_call_result_n7, hxc_tmp_call_result_n9, hxc_tmp_call_result_n11);
    struct hxc_caxecraft_domain_BlockCoord hxc_coordinate = hxc_tmp_call_result_n12;
    int32_t hxc_tmp_call_result_n14 = hxc_caxecraft_content_FirstPlayableLevel_fluidKind(hxc_fluidIndex);
    bool hxc_tmp_conditional_result_n5 = false;
    if (hxc_tmp_call_result_n14 == 0)
    {
      struct hxc_caxecraft_domain_BlockCoord hxc_tmp_load_result_n15 = hxc_coordinate;
      int32_t hxc_tmp_call_result_n17 = hxc_caxecraft_content_FirstPlayableLevel_fluidWidth(hxc_fluidIndex);
      int32_t hxc_tmp_call_result_n19 = hxc_caxecraft_content_FirstPlayableLevel_fluidHeight(hxc_fluidIndex);
      int32_t hxc_tmp_call_result_n21 = hxc_caxecraft_content_FirstPlayableLevel_fluidDepth(hxc_fluidIndex);
      bool hxc_tmp_instance_call_result_n22 = hxc_caxecraft_domain_GameSession_placeInitialWaterVolume(hxc_session, hxc_tmp_load_result_n15, hxc_tmp_call_result_n17, hxc_tmp_call_result_n19, hxc_tmp_call_result_n21);
      hxc_tmp_conditional_result_n5 = hxc_tmp_instance_call_result_n22;
    }
    else
    {
      bool hxc_tmp_instance_call_result_n24 = hxc_caxecraft_domain_GameSession_placeWaterSource(hxc_session, hxc_coordinate);
      hxc_tmp_conditional_result_n5 = hxc_tmp_instance_call_result_n24;
    }
    bool hxc_loaded = hxc_tmp_conditional_result_n5;
    if (!!hxc_loaded)
    {
      hxc_fluidIndex = hxc_i32_add_wrapping(hxc_fluidIndex, 1);
      continue;
    }
    return (struct hxc_caxecraft_content_FirstPlayableSessionLoad){ .hxc_valid = false, .hxc_waterPresentationCell = -1 };
  }
  int32_t hxc_itemIndex = 0;
  while (1)
  {
    if (!(hxc_itemIndex < 1))
    {
      break;
    }
    bool hxc_tmp_instance_call_result_n31 = hxc_caxecraft_domain_GameSession_activateAuthoredItemDuringLoad(hxc_session, hxc_itemIndex);
    if (!!hxc_tmp_instance_call_result_n31)
    {
      hxc_itemIndex = hxc_i32_add_wrapping(hxc_itemIndex, 1);
      continue;
    }
    return (struct hxc_caxecraft_content_FirstPlayableSessionLoad){ .hxc_valid = false, .hxc_waterPresentationCell = -1 };
  }
  return (struct hxc_caxecraft_content_FirstPlayableSessionLoad){ .hxc_valid = true, .hxc_waterPresentationCell = hxc_presentationCell };
}

int32_t hxc_caxecraft_content_FirstPlayableSessionLoader_validateFacts(void)
{
  int32_t hxc_itemIndex = 0;
  while (1)
  {
    if (!(hxc_itemIndex < 1))
    {
      break;
    }
    bool hxc_tmp = false;
    int32_t hxc_tmp_call_result_n2 = hxc_caxecraft_content_FirstPlayableLevel_itemStorageCode(hxc_itemIndex);
    int32_t hxc_code = hxc_tmp_call_result_n2;
    int32_t hxc_tmp_load_result_n3 = hxc_code;
    bool hxc_tmp_short_circuit_result_n3 = hxc_tmp_load_result_n3 >= 0;
    if (hxc_tmp_load_result_n3 >= 0)
    {
      hxc_tmp_short_circuit_result_n3 = hxc_code < 9;
    }
    bool hxc_tmp_short_circuit_load_result_n5 = hxc_tmp_short_circuit_result_n3;
    bool hxc_tmp_short_circuit_result_n4 = !hxc_tmp_short_circuit_load_result_n5;
    if (!!hxc_tmp_short_circuit_load_result_n5)
    {
      int32_t hxc_tmp_call_result_n7 = hxc_caxecraft_content_FirstPlayableLevel_itemQuantity(hxc_itemIndex);
      hxc_tmp_short_circuit_result_n4 = hxc_tmp_call_result_n7 <= 0;
    }
    if (!hxc_tmp_short_circuit_result_n4)
    {
      int32_t hxc_tmp_call_result_n10 = hxc_caxecraft_content_FirstPlayableLevel_itemXMilli(hxc_itemIndex);
      int32_t hxc_x_hea40d1a30f46 = hxc_tmp_call_result_n10;
      int32_t hxc_tmp_call_result_n12 = hxc_caxecraft_content_FirstPlayableLevel_itemYMilli(hxc_itemIndex);
      int32_t hxc_y_hf30f99d7ab72 = hxc_tmp_call_result_n12;
      int32_t hxc_tmp_call_result_n14 = hxc_caxecraft_content_FirstPlayableLevel_itemZMilli(hxc_itemIndex);
      int32_t hxc_z_hda052bd86333 = hxc_tmp_call_result_n14;
      int32_t hxc_tmp_load_result_n15 = hxc_x_hea40d1a30f46;
      bool hxc_tmp_short_circuit_result_n8 = hxc_tmp_load_result_n15 >= 0;
      if (hxc_tmp_load_result_n15 >= 0)
      {
        hxc_tmp_short_circuit_result_n8 = hxc_x_hea40d1a30f46 < 32000;
      }
      bool hxc_tmp_short_circuit_load_result_n17 = hxc_tmp_short_circuit_result_n8;
      bool hxc_tmp_short_circuit_result_n9 = hxc_tmp_short_circuit_load_result_n17;
      if (hxc_tmp_short_circuit_load_result_n17)
      {
        hxc_tmp_short_circuit_result_n9 = hxc_y_hf30f99d7ab72 >= 0;
      }
      bool hxc_tmp_short_circuit_load_result_n19 = hxc_tmp_short_circuit_result_n9;
      bool hxc_tmp_short_circuit_result_n10 = hxc_tmp_short_circuit_load_result_n19;
      if (hxc_tmp_short_circuit_load_result_n19)
      {
        hxc_tmp_short_circuit_result_n10 = hxc_y_hf30f99d7ab72 < 16000;
      }
      bool hxc_tmp_short_circuit_load_result_n21 = hxc_tmp_short_circuit_result_n10;
      bool hxc_tmp_short_circuit_result_n11 = hxc_tmp_short_circuit_load_result_n21;
      if (hxc_tmp_short_circuit_load_result_n21)
      {
        hxc_tmp_short_circuit_result_n11 = hxc_z_hda052bd86333 >= 0;
      }
      bool hxc_tmp_short_circuit_load_result_n23 = hxc_tmp_short_circuit_result_n11;
      bool hxc_tmp_short_circuit_result_n12 = hxc_tmp_short_circuit_load_result_n23;
      if (hxc_tmp_short_circuit_load_result_n23)
      {
        hxc_tmp_short_circuit_result_n12 = hxc_z_hda052bd86333 < 32000;
      }
      hxc_tmp = !hxc_tmp_short_circuit_result_n12;
    }
    else
    {
      hxc_tmp = true;
    }
    if (!hxc_tmp)
    {
      hxc_itemIndex = hxc_i32_add_wrapping(hxc_itemIndex, 1);
      continue;
    }
    return -1;
  }
  int32_t hxc_presentationCell = -1;
  int32_t hxc_fluidIndex = 0;
  while (1)
  {
    if (!(hxc_fluidIndex < 2))
    {
      break;
    }
    int32_t hxc_tmp_call_result_n30 = hxc_caxecraft_content_FirstPlayableLevel_fluidKind(hxc_fluidIndex);
    int32_t hxc_kind = hxc_tmp_call_result_n30;
    int32_t hxc_tmp_call_result_n32 = hxc_caxecraft_content_FirstPlayableLevel_fluidWidth(hxc_fluidIndex);
    int32_t hxc_width = hxc_tmp_call_result_n32;
    int32_t hxc_tmp_call_result_n34 = hxc_caxecraft_content_FirstPlayableLevel_fluidHeight(hxc_fluidIndex);
    int32_t hxc_height = hxc_tmp_call_result_n34;
    int32_t hxc_tmp_call_result_n36 = hxc_caxecraft_content_FirstPlayableLevel_fluidDepth(hxc_fluidIndex);
    int32_t hxc_depth = hxc_tmp_call_result_n36;
    int32_t hxc_tmp_call_result_n38 = hxc_caxecraft_content_FirstPlayableLevel_fluidX(hxc_fluidIndex);
    int32_t hxc_x_h4795f1b28989 = hxc_tmp_call_result_n38;
    int32_t hxc_tmp_call_result_n40 = hxc_caxecraft_content_FirstPlayableLevel_fluidY(hxc_fluidIndex);
    int32_t hxc_y_h13412ed2aff9 = hxc_tmp_call_result_n40;
    int32_t hxc_tmp_call_result_n42 = hxc_caxecraft_content_FirstPlayableLevel_fluidZ(hxc_fluidIndex);
    int32_t hxc_z_h200cb7f017d1 = hxc_tmp_call_result_n42;
    int32_t hxc_tmp_call_result_n44 = hxc_caxecraft_content_FirstPlayableLevel_fluidPresentationCell(hxc_fluidIndex);
    int32_t hxc_cell = hxc_tmp_call_result_n44;
    int32_t hxc_tmp_load_result_n45 = hxc_kind;
    bool hxc_tmp_short_circuit_result_n23 = hxc_tmp_load_result_n45 != 0;
    if (hxc_tmp_load_result_n45 != 0)
    {
      hxc_tmp_short_circuit_result_n23 = hxc_kind != 1;
    }
    bool hxc_tmp_short_circuit_load_result_n47 = hxc_tmp_short_circuit_result_n23;
    bool hxc_tmp_short_circuit_result_n24 = hxc_tmp_short_circuit_load_result_n47;
    if (!hxc_tmp_short_circuit_load_result_n47)
    {
      hxc_tmp_short_circuit_result_n24 = hxc_width <= 0;
    }
    bool hxc_tmp_short_circuit_load_result_n49 = hxc_tmp_short_circuit_result_n24;
    bool hxc_tmp_short_circuit_result_n25 = hxc_tmp_short_circuit_load_result_n49;
    if (!hxc_tmp_short_circuit_load_result_n49)
    {
      hxc_tmp_short_circuit_result_n25 = hxc_height <= 0;
    }
    bool hxc_tmp_short_circuit_load_result_n51 = hxc_tmp_short_circuit_result_n25;
    bool hxc_tmp_short_circuit_result_n26 = hxc_tmp_short_circuit_load_result_n51;
    if (!hxc_tmp_short_circuit_load_result_n51)
    {
      hxc_tmp_short_circuit_result_n26 = hxc_depth <= 0;
    }
    bool hxc_tmp_short_circuit_load_result_n53 = hxc_tmp_short_circuit_result_n26;
    bool hxc_tmp_short_circuit_result_n27 = hxc_tmp_short_circuit_load_result_n53;
    if (!hxc_tmp_short_circuit_load_result_n53)
    {
      hxc_tmp_short_circuit_result_n27 = hxc_x_h4795f1b28989 < 0;
    }
    bool hxc_tmp_short_circuit_load_result_n55 = hxc_tmp_short_circuit_result_n27;
    bool hxc_tmp_short_circuit_result_n28 = hxc_tmp_short_circuit_load_result_n55;
    if (!hxc_tmp_short_circuit_load_result_n55)
    {
      hxc_tmp_short_circuit_result_n28 = hxc_y_h13412ed2aff9 < 0;
    }
    bool hxc_tmp_short_circuit_load_result_n57 = hxc_tmp_short_circuit_result_n28;
    bool hxc_tmp_short_circuit_result_n29 = hxc_tmp_short_circuit_load_result_n57;
    if (!hxc_tmp_short_circuit_load_result_n57)
    {
      hxc_tmp_short_circuit_result_n29 = hxc_z_h200cb7f017d1 < 0;
    }
    bool hxc_tmp_short_circuit_load_result_n59 = hxc_tmp_short_circuit_result_n29;
    bool hxc_tmp_short_circuit_result_n30 = hxc_tmp_short_circuit_load_result_n59;
    if (!hxc_tmp_short_circuit_load_result_n59)
    {
      int32_t hxc_tmp_load_result_n60 = hxc_x_h4795f1b28989;
      hxc_tmp_short_circuit_result_n30 = hxc_tmp_load_result_n60 > hxc_i32_subtract_wrapping(32, hxc_width);
    }
    bool hxc_tmp_short_circuit_load_result_n62 = hxc_tmp_short_circuit_result_n30;
    bool hxc_tmp_short_circuit_result_n31 = hxc_tmp_short_circuit_load_result_n62;
    if (!hxc_tmp_short_circuit_load_result_n62)
    {
      int32_t hxc_tmp_load_result_n63 = hxc_y_h13412ed2aff9;
      hxc_tmp_short_circuit_result_n31 = hxc_tmp_load_result_n63 > hxc_i32_subtract_wrapping(16, hxc_height);
    }
    bool hxc_tmp_short_circuit_load_result_n65 = hxc_tmp_short_circuit_result_n31;
    bool hxc_tmp_short_circuit_result_n32 = hxc_tmp_short_circuit_load_result_n65;
    if (!hxc_tmp_short_circuit_load_result_n65)
    {
      int32_t hxc_tmp_load_result_n66 = hxc_z_h200cb7f017d1;
      hxc_tmp_short_circuit_result_n32 = hxc_tmp_load_result_n66 > hxc_i32_subtract_wrapping(32, hxc_depth);
    }
    bool hxc_tmp_short_circuit_load_result_n68 = hxc_tmp_short_circuit_result_n32;
    bool hxc_tmp_short_circuit_result_n33 = hxc_tmp_short_circuit_load_result_n68;
    if (!hxc_tmp_short_circuit_load_result_n68)
    {
      hxc_tmp_short_circuit_result_n33 = hxc_cell < 0;
    }
    if (!hxc_tmp_short_circuit_result_n33)
    {
      if (hxc_presentationCell < 0)
      {
        hxc_presentationCell = hxc_cell;
      }
      else
      {
        int32_t hxc_tmp_load_result_n73 = hxc_presentationCell;
        if (hxc_tmp_load_result_n73 != hxc_cell)
        {
          return -1;
        }
      }
      hxc_fluidIndex = hxc_i32_add_wrapping(hxc_fluidIndex, 1);
      continue;
    }
    return -1;
  }
  return hxc_presentationCell;
}
