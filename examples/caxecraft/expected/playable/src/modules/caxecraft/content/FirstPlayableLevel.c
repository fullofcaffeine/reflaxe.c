#include "hxc/program.h"

int32_t hxc_caxecraft_content_FirstPlayableLevel_fluidDepth(int32_t hxc_index)
{
  if (!(hxc_index == 0))
  {
    if (!(hxc_index == 1))
    {
      return -1;
    }
    return 1;
  }
  return 4;
}

int32_t hxc_caxecraft_content_FirstPlayableLevel_fluidHeight(int32_t hxc_index)
{
  if (!(hxc_index == 0))
  {
    if (!(hxc_index == 1))
    {
      return -1;
    }
    return 1;
  }
  return 1;
}

int32_t hxc_caxecraft_content_FirstPlayableLevel_fluidKind(int32_t hxc_index)
{
  if (!(hxc_index == 0))
  {
    if (!(hxc_index == 1))
    {
      return -1;
    }
    return 1;
  }
  return 0;
}

int32_t hxc_caxecraft_content_FirstPlayableLevel_fluidPresentationCell(int32_t hxc_index)
{
  if (!(hxc_index == 0))
  {
    if (!(hxc_index == 1))
    {
      return -1;
    }
    return 5;
  }
  return 5;
}

int32_t hxc_caxecraft_content_FirstPlayableLevel_fluidWidth(int32_t hxc_index)
{
  if (!(hxc_index == 0))
  {
    if (!(hxc_index == 1))
    {
      return -1;
    }
    return 1;
  }
  return 4;
}

int32_t hxc_caxecraft_content_FirstPlayableLevel_fluidX(int32_t hxc_index)
{
  if (!(hxc_index == 0))
  {
    if (!(hxc_index == 1))
    {
      return -1;
    }
    return 3;
  }
  return 2;
}

int32_t hxc_caxecraft_content_FirstPlayableLevel_fluidY(int32_t hxc_index)
{
  if (!(hxc_index == 0))
  {
    if (!(hxc_index == 1))
    {
      return -1;
    }
    return 4;
  }
  return 4;
}

int32_t hxc_caxecraft_content_FirstPlayableLevel_fluidZ(int32_t hxc_index)
{
  if (!(hxc_index == 0))
  {
    if (!(hxc_index == 1))
    {
      return -1;
    }
    return 3;
  }
  return 2;
}

bool hxc_caxecraft_content_FirstPlayableLevel_loadTerrain(uint8_t *hxc_cells, size_t hxc_length)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n1 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n1;
  int32_t hxc_index = 0;
  int32_t hxc_tmp_call_result_n1 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n1;
  int32_t hxc_tmp_call_result_n3 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n3;
  int32_t hxc_tmp_call_result_n5 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n5;
  int32_t hxc_tmp_call_result_n7 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n7;
  int32_t hxc_tmp_call_result_n9 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n9;
  int32_t hxc_tmp_call_result_n11 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n11;
  int32_t hxc_tmp_call_result_n13 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n13;
  int32_t hxc_tmp_call_result_n15 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n15;
  int32_t hxc_tmp_call_result_n17 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n17;
  int32_t hxc_tmp_call_result_n19 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n19;
  int32_t hxc_tmp_call_result_n21 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 2);
  hxc_index = hxc_tmp_call_result_n21;
  int32_t hxc_tmp_call_result_n23 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 4);
  hxc_index = hxc_tmp_call_result_n23;
  int32_t hxc_tmp_call_result_n25 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 26);
  hxc_index = hxc_tmp_call_result_n25;
  int32_t hxc_tmp_call_result_n27 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n27;
  int32_t hxc_tmp_call_result_n29 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n29;
  int32_t hxc_tmp_call_result_n31 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n31;
  int32_t hxc_tmp_call_result_n33 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 2);
  hxc_index = hxc_tmp_call_result_n33;
  int32_t hxc_tmp_call_result_n35 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 4);
  hxc_index = hxc_tmp_call_result_n35;
  int32_t hxc_tmp_call_result_n37 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 26);
  hxc_index = hxc_tmp_call_result_n37;
  int32_t hxc_tmp_call_result_n39 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n39;
  int32_t hxc_tmp_call_result_n41 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n41;
  int32_t hxc_tmp_call_result_n43 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n43;
  int32_t hxc_tmp_call_result_n45 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 2);
  hxc_index = hxc_tmp_call_result_n45;
  int32_t hxc_tmp_call_result_n47 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 4);
  hxc_index = hxc_tmp_call_result_n47;
  int32_t hxc_tmp_call_result_n49 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 26);
  hxc_index = hxc_tmp_call_result_n49;
  int32_t hxc_tmp_call_result_n51 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n51;
  int32_t hxc_tmp_call_result_n53 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n53;
  int32_t hxc_tmp_call_result_n55 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n55;
  int32_t hxc_tmp_call_result_n57 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 2);
  hxc_index = hxc_tmp_call_result_n57;
  int32_t hxc_tmp_call_result_n59 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 4);
  hxc_index = hxc_tmp_call_result_n59;
  int32_t hxc_tmp_call_result_n61 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 26);
  hxc_index = hxc_tmp_call_result_n61;
  int32_t hxc_tmp_call_result_n63 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n63;
  int32_t hxc_tmp_call_result_n65 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n65;
  int32_t hxc_tmp_call_result_n67 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n67;
  int32_t hxc_tmp_call_result_n69 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n69;
  int32_t hxc_tmp_call_result_n71 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n71;
  int32_t hxc_tmp_call_result_n73 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n73;
  int32_t hxc_tmp_call_result_n75 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n75;
  int32_t hxc_tmp_call_result_n77 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n77;
  int32_t hxc_tmp_call_result_n79 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n79;
  int32_t hxc_tmp_call_result_n81 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n81;
  int32_t hxc_tmp_call_result_n83 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n83;
  int32_t hxc_tmp_call_result_n85 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n85;
  int32_t hxc_tmp_call_result_n87 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n87;
  int32_t hxc_tmp_call_result_n89 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n89;
  int32_t hxc_tmp_call_result_n91 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n91;
  int32_t hxc_tmp_call_result_n93 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n93;
  int32_t hxc_tmp_call_result_n95 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n95;
  int32_t hxc_tmp_call_result_n97 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n97;
  int32_t hxc_tmp_call_result_n99 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n99;
  int32_t hxc_tmp_call_result_n101 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n101;
  int32_t hxc_tmp_call_result_n103 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n103;
  int32_t hxc_tmp_call_result_n105 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n105;
  int32_t hxc_tmp_call_result_n107 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n107;
  int32_t hxc_tmp_call_result_n109 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n109;
  int32_t hxc_tmp_call_result_n111 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n111;
  int32_t hxc_tmp_call_result_n113 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n113;
  int32_t hxc_tmp_call_result_n115 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n115;
  int32_t hxc_tmp_call_result_n117 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n117;
  int32_t hxc_tmp_call_result_n119 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n119;
  int32_t hxc_tmp_call_result_n121 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n121;
  int32_t hxc_tmp_call_result_n123 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n123;
  int32_t hxc_tmp_call_result_n125 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n125;
  int32_t hxc_tmp_call_result_n127 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n127;
  int32_t hxc_tmp_call_result_n129 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n129;
  int32_t hxc_tmp_call_result_n131 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n131;
  int32_t hxc_tmp_call_result_n133 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n133;
  int32_t hxc_tmp_call_result_n135 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n135;
  int32_t hxc_tmp_call_result_n137 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n137;
  int32_t hxc_tmp_call_result_n139 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n139;
  int32_t hxc_tmp_call_result_n141 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n141;
  int32_t hxc_tmp_call_result_n143 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n143;
  int32_t hxc_tmp_call_result_n145 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n145;
  int32_t hxc_tmp_call_result_n147 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n147;
  int32_t hxc_tmp_call_result_n149 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n149;
  int32_t hxc_tmp_call_result_n151 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n151;
  int32_t hxc_tmp_call_result_n153 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n153;
  int32_t hxc_tmp_call_result_n155 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n155;
  int32_t hxc_tmp_call_result_n157 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n157;
  int32_t hxc_tmp_call_result_n159 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n159;
  int32_t hxc_tmp_call_result_n161 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n161;
  int32_t hxc_tmp_call_result_n163 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n163;
  int32_t hxc_tmp_call_result_n165 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n165;
  int32_t hxc_tmp_call_result_n167 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n167;
  int32_t hxc_tmp_call_result_n169 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n169;
  int32_t hxc_tmp_call_result_n171 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n171;
  int32_t hxc_tmp_call_result_n173 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n173;
  int32_t hxc_tmp_call_result_n175 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n175;
  int32_t hxc_tmp_call_result_n177 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n177;
  int32_t hxc_tmp_call_result_n179 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n179;
  int32_t hxc_tmp_call_result_n181 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n181;
  int32_t hxc_tmp_call_result_n183 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n183;
  int32_t hxc_tmp_call_result_n185 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n185;
  int32_t hxc_tmp_call_result_n187 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n187;
  int32_t hxc_tmp_call_result_n189 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n189;
  int32_t hxc_tmp_call_result_n191 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n191;
  int32_t hxc_tmp_call_result_n193 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n193;
  int32_t hxc_tmp_call_result_n195 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n195;
  int32_t hxc_tmp_call_result_n197 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n197;
  int32_t hxc_tmp_call_result_n199 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n199;
  int32_t hxc_tmp_call_result_n201 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n201;
  int32_t hxc_tmp_call_result_n203 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n203;
  int32_t hxc_tmp_call_result_n205 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n205;
  int32_t hxc_tmp_call_result_n207 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n207;
  int32_t hxc_tmp_call_result_n209 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n209;
  int32_t hxc_tmp_call_result_n211 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n211;
  int32_t hxc_tmp_call_result_n213 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n213;
  int32_t hxc_tmp_call_result_n215 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n215;
  int32_t hxc_tmp_call_result_n217 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n217;
  int32_t hxc_tmp_call_result_n219 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n219;
  int32_t hxc_tmp_call_result_n221 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n221;
  int32_t hxc_tmp_call_result_n223 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n223;
  int32_t hxc_tmp_call_result_n225 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n225;
  int32_t hxc_tmp_call_result_n227 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n227;
  int32_t hxc_tmp_call_result_n229 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n229;
  int32_t hxc_tmp_call_result_n231 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n231;
  int32_t hxc_tmp_call_result_n233 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n233;
  int32_t hxc_tmp_call_result_n235 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n235;
  int32_t hxc_tmp_call_result_n237 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n237;
  int32_t hxc_tmp_call_result_n239 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n239;
  int32_t hxc_tmp_call_result_n241 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n241;
  int32_t hxc_tmp_call_result_n243 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n243;
  int32_t hxc_tmp_call_result_n245 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n245;
  int32_t hxc_tmp_call_result_n247 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n247;
  int32_t hxc_tmp_call_result_n249 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n249;
  int32_t hxc_tmp_call_result_n251 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n251;
  int32_t hxc_tmp_call_result_n253 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n253;
  int32_t hxc_tmp_call_result_n255 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n255;
  int32_t hxc_tmp_call_result_n257 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n257;
  int32_t hxc_tmp_call_result_n259 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n259;
  int32_t hxc_tmp_call_result_n261 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n261;
  int32_t hxc_tmp_call_result_n263 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n263;
  int32_t hxc_tmp_call_result_n265 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n265;
  int32_t hxc_tmp_call_result_n267 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n267;
  int32_t hxc_tmp_call_result_n269 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n269;
  int32_t hxc_tmp_call_result_n271 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_cells, hxc_length, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n271;
  return hxc_index == 16384;
}

int32_t hxc_caxecraft_content_FirstPlayableLevel_writeRun(uint8_t *hxc_cells, size_t hxc_length, int32_t hxc_start, int32_t hxc_code, int32_t hxc_count)
{
  uint8_t *hxc_borrow = hxc_cells;
  size_t hxc_tmp_length_n4 = hxc_length;
  (void)hxc_borrow;
  (void)hxc_tmp_length_n4;
  int32_t hxc_index = hxc_start;
  int32_t hxc_end = hxc_i32_add_wrapping(hxc_start, hxc_count);
  while (1)
  {
    int32_t hxc_tmp_load_result_n0 = hxc_index;
    if (!(hxc_tmp_load_result_n0 < hxc_end))
    {
      break;
    }
    hxc_caxecraft_domain_WorldStorage_writeCode(hxc_cells, hxc_length, hxc_index, hxc_code);
    hxc_index = hxc_i32_add_wrapping(hxc_index, 1);
  }
  return hxc_index;
}
