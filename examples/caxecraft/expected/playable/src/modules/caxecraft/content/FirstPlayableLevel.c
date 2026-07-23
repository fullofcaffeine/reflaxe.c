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

int32_t hxc_caxecraft_content_FirstPlayableLevel_itemQuantity(int32_t hxc_index)
{
  if (!(hxc_index == 0))
  {
    return -1;
  }
  return 1;
}

int32_t hxc_caxecraft_content_FirstPlayableLevel_itemStorageCode(int32_t hxc_index)
{
  if (!(hxc_index == 0))
  {
    return -1;
  }
  return 8;
}

int32_t hxc_caxecraft_content_FirstPlayableLevel_itemXMilli(int32_t hxc_index)
{
  if (!(hxc_index == 0))
  {
    return -1;
  }
  return 4500;
}

int32_t hxc_caxecraft_content_FirstPlayableLevel_itemYMilli(int32_t hxc_index)
{
  if (!(hxc_index == 0))
  {
    return -1;
  }
  return 5000;
}

int32_t hxc_caxecraft_content_FirstPlayableLevel_itemZMilli(int32_t hxc_index)
{
  if (!(hxc_index == 0))
  {
    return -1;
  }
  return 4500;
}

bool hxc_caxecraft_content_FirstPlayableLevel_loadTerrain(struct hxc_caxecraft_domain_GameSession *hxc_session)
{
  int32_t hxc_index = 0;
  int32_t hxc_tmp_call_result_n1 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n1;
  int32_t hxc_tmp_call_result_n3 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n3;
  int32_t hxc_tmp_call_result_n5 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n5;
  int32_t hxc_tmp_call_result_n7 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n7;
  int32_t hxc_tmp_call_result_n9 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n9;
  int32_t hxc_tmp_call_result_n11 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n11;
  int32_t hxc_tmp_call_result_n13 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 1);
  hxc_index = hxc_tmp_call_result_n13;
  int32_t hxc_tmp_call_result_n15 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 5, 6);
  hxc_index = hxc_tmp_call_result_n15;
  int32_t hxc_tmp_call_result_n17 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 25);
  hxc_index = hxc_tmp_call_result_n17;
  int32_t hxc_tmp_call_result_n19 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n19;
  int32_t hxc_tmp_call_result_n21 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n21;
  int32_t hxc_tmp_call_result_n23 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n23;
  int32_t hxc_tmp_call_result_n25 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 5, 2);
  hxc_index = hxc_tmp_call_result_n25;
  int32_t hxc_tmp_call_result_n27 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 4);
  hxc_index = hxc_tmp_call_result_n27;
  int32_t hxc_tmp_call_result_n29 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 5, 2);
  hxc_index = hxc_tmp_call_result_n29;
  int32_t hxc_tmp_call_result_n31 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 24);
  hxc_index = hxc_tmp_call_result_n31;
  int32_t hxc_tmp_call_result_n33 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n33;
  int32_t hxc_tmp_call_result_n35 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n35;
  int32_t hxc_tmp_call_result_n37 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n37;
  int32_t hxc_tmp_call_result_n39 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 5, 2);
  hxc_index = hxc_tmp_call_result_n39;
  int32_t hxc_tmp_call_result_n41 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 4);
  hxc_index = hxc_tmp_call_result_n41;
  int32_t hxc_tmp_call_result_n43 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 5, 2);
  hxc_index = hxc_tmp_call_result_n43;
  int32_t hxc_tmp_call_result_n45 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 24);
  hxc_index = hxc_tmp_call_result_n45;
  int32_t hxc_tmp_call_result_n47 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n47;
  int32_t hxc_tmp_call_result_n49 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n49;
  int32_t hxc_tmp_call_result_n51 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n51;
  int32_t hxc_tmp_call_result_n53 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 5, 2);
  hxc_index = hxc_tmp_call_result_n53;
  int32_t hxc_tmp_call_result_n55 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 4);
  hxc_index = hxc_tmp_call_result_n55;
  int32_t hxc_tmp_call_result_n57 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 5, 2);
  hxc_index = hxc_tmp_call_result_n57;
  int32_t hxc_tmp_call_result_n59 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 24);
  hxc_index = hxc_tmp_call_result_n59;
  int32_t hxc_tmp_call_result_n61 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n61;
  int32_t hxc_tmp_call_result_n63 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n63;
  int32_t hxc_tmp_call_result_n65 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n65;
  int32_t hxc_tmp_call_result_n67 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 5, 2);
  hxc_index = hxc_tmp_call_result_n67;
  int32_t hxc_tmp_call_result_n69 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 4);
  hxc_index = hxc_tmp_call_result_n69;
  int32_t hxc_tmp_call_result_n71 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 5, 2);
  hxc_index = hxc_tmp_call_result_n71;
  int32_t hxc_tmp_call_result_n73 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 24);
  hxc_index = hxc_tmp_call_result_n73;
  int32_t hxc_tmp_call_result_n75 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n75;
  int32_t hxc_tmp_call_result_n77 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n77;
  int32_t hxc_tmp_call_result_n79 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n79;
  int32_t hxc_tmp_call_result_n81 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 1);
  hxc_index = hxc_tmp_call_result_n81;
  int32_t hxc_tmp_call_result_n83 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 5, 6);
  hxc_index = hxc_tmp_call_result_n83;
  int32_t hxc_tmp_call_result_n85 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 25);
  hxc_index = hxc_tmp_call_result_n85;
  int32_t hxc_tmp_call_result_n87 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n87;
  int32_t hxc_tmp_call_result_n89 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n89;
  int32_t hxc_tmp_call_result_n91 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n91;
  int32_t hxc_tmp_call_result_n93 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n93;
  int32_t hxc_tmp_call_result_n95 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n95;
  int32_t hxc_tmp_call_result_n97 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n97;
  int32_t hxc_tmp_call_result_n99 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n99;
  int32_t hxc_tmp_call_result_n101 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n101;
  int32_t hxc_tmp_call_result_n103 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n103;
  int32_t hxc_tmp_call_result_n105 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n105;
  int32_t hxc_tmp_call_result_n107 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n107;
  int32_t hxc_tmp_call_result_n109 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n109;
  int32_t hxc_tmp_call_result_n111 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 89);
  hxc_index = hxc_tmp_call_result_n111;
  int32_t hxc_tmp_call_result_n113 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 7, 3);
  hxc_index = hxc_tmp_call_result_n113;
  int32_t hxc_tmp_call_result_n115 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 260);
  hxc_index = hxc_tmp_call_result_n115;
  int32_t hxc_tmp_call_result_n117 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n117;
  int32_t hxc_tmp_call_result_n119 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n119;
  int32_t hxc_tmp_call_result_n121 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n121;
  int32_t hxc_tmp_call_result_n123 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 26);
  hxc_index = hxc_tmp_call_result_n123;
  int32_t hxc_tmp_call_result_n125 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 6, 1);
  hxc_index = hxc_tmp_call_result_n125;
  int32_t hxc_tmp_call_result_n127 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 31);
  hxc_index = hxc_tmp_call_result_n127;
  int32_t hxc_tmp_call_result_n129 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 6, 1);
  hxc_index = hxc_tmp_call_result_n129;
  int32_t hxc_tmp_call_result_n131 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 30);
  hxc_index = hxc_tmp_call_result_n131;
  int32_t hxc_tmp_call_result_n133 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 7, 3);
  hxc_index = hxc_tmp_call_result_n133;
  int32_t hxc_tmp_call_result_n135 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 30);
  hxc_index = hxc_tmp_call_result_n135;
  int32_t hxc_tmp_call_result_n137 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 7, 1);
  hxc_index = hxc_tmp_call_result_n137;
  int32_t hxc_tmp_call_result_n139 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 229);
  hxc_index = hxc_tmp_call_result_n139;
  int32_t hxc_tmp_call_result_n141 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n141;
  int32_t hxc_tmp_call_result_n143 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n143;
  int32_t hxc_tmp_call_result_n145 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n145;
  int32_t hxc_tmp_call_result_n147 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 89);
  hxc_index = hxc_tmp_call_result_n147;
  int32_t hxc_tmp_call_result_n149 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 7, 3);
  hxc_index = hxc_tmp_call_result_n149;
  int32_t hxc_tmp_call_result_n151 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 260);
  hxc_index = hxc_tmp_call_result_n151;
  int32_t hxc_tmp_call_result_n153 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n153;
  int32_t hxc_tmp_call_result_n155 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n155;
  int32_t hxc_tmp_call_result_n157 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n157;
  int32_t hxc_tmp_call_result_n159 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n159;
  int32_t hxc_tmp_call_result_n161 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n161;
  int32_t hxc_tmp_call_result_n163 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n163;
  int32_t hxc_tmp_call_result_n165 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n165;
  int32_t hxc_tmp_call_result_n167 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n167;
  int32_t hxc_tmp_call_result_n169 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n169;
  int32_t hxc_tmp_call_result_n171 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n171;
  int32_t hxc_tmp_call_result_n173 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n173;
  int32_t hxc_tmp_call_result_n175 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n175;
  int32_t hxc_tmp_call_result_n177 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n177;
  int32_t hxc_tmp_call_result_n179 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n179;
  int32_t hxc_tmp_call_result_n181 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n181;
  int32_t hxc_tmp_call_result_n183 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n183;
  int32_t hxc_tmp_call_result_n185 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n185;
  int32_t hxc_tmp_call_result_n187 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n187;
  int32_t hxc_tmp_call_result_n189 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n189;
  int32_t hxc_tmp_call_result_n191 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n191;
  int32_t hxc_tmp_call_result_n193 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n193;
  int32_t hxc_tmp_call_result_n195 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n195;
  int32_t hxc_tmp_call_result_n197 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n197;
  int32_t hxc_tmp_call_result_n199 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n199;
  int32_t hxc_tmp_call_result_n201 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n201;
  int32_t hxc_tmp_call_result_n203 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n203;
  int32_t hxc_tmp_call_result_n205 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n205;
  int32_t hxc_tmp_call_result_n207 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n207;
  int32_t hxc_tmp_call_result_n209 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n209;
  int32_t hxc_tmp_call_result_n211 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n211;
  int32_t hxc_tmp_call_result_n213 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n213;
  int32_t hxc_tmp_call_result_n215 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n215;
  int32_t hxc_tmp_call_result_n217 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n217;
  int32_t hxc_tmp_call_result_n219 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n219;
  int32_t hxc_tmp_call_result_n221 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n221;
  int32_t hxc_tmp_call_result_n223 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n223;
  int32_t hxc_tmp_call_result_n225 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n225;
  int32_t hxc_tmp_call_result_n227 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n227;
  int32_t hxc_tmp_call_result_n229 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n229;
  int32_t hxc_tmp_call_result_n231 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n231;
  int32_t hxc_tmp_call_result_n233 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n233;
  int32_t hxc_tmp_call_result_n235 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n235;
  int32_t hxc_tmp_call_result_n237 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n237;
  int32_t hxc_tmp_call_result_n239 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n239;
  int32_t hxc_tmp_call_result_n241 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n241;
  int32_t hxc_tmp_call_result_n243 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n243;
  int32_t hxc_tmp_call_result_n245 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 32);
  hxc_index = hxc_tmp_call_result_n245;
  int32_t hxc_tmp_call_result_n247 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n247;
  int32_t hxc_tmp_call_result_n249 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n249;
  int32_t hxc_tmp_call_result_n251 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n251;
  int32_t hxc_tmp_call_result_n253 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 8, 10);
  hxc_index = hxc_tmp_call_result_n253;
  int32_t hxc_tmp_call_result_n255 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 12);
  hxc_index = hxc_tmp_call_result_n255;
  int32_t hxc_tmp_call_result_n257 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 9, 10);
  hxc_index = hxc_tmp_call_result_n257;
  int32_t hxc_tmp_call_result_n259 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n259;
  int32_t hxc_tmp_call_result_n261 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n261;
  int32_t hxc_tmp_call_result_n263 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n263;
  int32_t hxc_tmp_call_result_n265 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 8, 10);
  hxc_index = hxc_tmp_call_result_n265;
  int32_t hxc_tmp_call_result_n267 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 12);
  hxc_index = hxc_tmp_call_result_n267;
  int32_t hxc_tmp_call_result_n269 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 9, 10);
  hxc_index = hxc_tmp_call_result_n269;
  int32_t hxc_tmp_call_result_n271 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n271;
  int32_t hxc_tmp_call_result_n273 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n273;
  int32_t hxc_tmp_call_result_n275 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n275;
  int32_t hxc_tmp_call_result_n277 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 8, 10);
  hxc_index = hxc_tmp_call_result_n277;
  int32_t hxc_tmp_call_result_n279 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 12);
  hxc_index = hxc_tmp_call_result_n279;
  int32_t hxc_tmp_call_result_n281 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 9, 10);
  hxc_index = hxc_tmp_call_result_n281;
  int32_t hxc_tmp_call_result_n283 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n283;
  int32_t hxc_tmp_call_result_n285 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n285;
  int32_t hxc_tmp_call_result_n287 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n287;
  int32_t hxc_tmp_call_result_n289 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 8, 10);
  hxc_index = hxc_tmp_call_result_n289;
  int32_t hxc_tmp_call_result_n291 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 12);
  hxc_index = hxc_tmp_call_result_n291;
  int32_t hxc_tmp_call_result_n293 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 9, 10);
  hxc_index = hxc_tmp_call_result_n293;
  int32_t hxc_tmp_call_result_n295 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n295;
  int32_t hxc_tmp_call_result_n297 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n297;
  int32_t hxc_tmp_call_result_n299 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n299;
  int32_t hxc_tmp_call_result_n301 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 8, 10);
  hxc_index = hxc_tmp_call_result_n301;
  int32_t hxc_tmp_call_result_n303 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 12);
  hxc_index = hxc_tmp_call_result_n303;
  int32_t hxc_tmp_call_result_n305 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 9, 10);
  hxc_index = hxc_tmp_call_result_n305;
  int32_t hxc_tmp_call_result_n307 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n307;
  int32_t hxc_tmp_call_result_n309 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n309;
  int32_t hxc_tmp_call_result_n311 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n311;
  int32_t hxc_tmp_call_result_n313 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 8, 10);
  hxc_index = hxc_tmp_call_result_n313;
  int32_t hxc_tmp_call_result_n315 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 12);
  hxc_index = hxc_tmp_call_result_n315;
  int32_t hxc_tmp_call_result_n317 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 9, 10);
  hxc_index = hxc_tmp_call_result_n317;
  int32_t hxc_tmp_call_result_n319 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n319;
  int32_t hxc_tmp_call_result_n321 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n321;
  int32_t hxc_tmp_call_result_n323 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n323;
  int32_t hxc_tmp_call_result_n325 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 8, 10);
  hxc_index = hxc_tmp_call_result_n325;
  int32_t hxc_tmp_call_result_n327 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 12);
  hxc_index = hxc_tmp_call_result_n327;
  int32_t hxc_tmp_call_result_n329 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 9, 10);
  hxc_index = hxc_tmp_call_result_n329;
  int32_t hxc_tmp_call_result_n331 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n331;
  int32_t hxc_tmp_call_result_n333 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 4, 32);
  hxc_index = hxc_tmp_call_result_n333;
  int32_t hxc_tmp_call_result_n335 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 2, 96);
  hxc_index = hxc_tmp_call_result_n335;
  int32_t hxc_tmp_call_result_n337 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 8, 10);
  hxc_index = hxc_tmp_call_result_n337;
  int32_t hxc_tmp_call_result_n339 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 1, 12);
  hxc_index = hxc_tmp_call_result_n339;
  int32_t hxc_tmp_call_result_n341 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 9, 10);
  hxc_index = hxc_tmp_call_result_n341;
  int32_t hxc_tmp_call_result_n343 = hxc_caxecraft_content_FirstPlayableLevel_writeRun(hxc_session, hxc_index, 0, 352);
  hxc_index = hxc_tmp_call_result_n343;
  return hxc_index == 16384;
}

int32_t hxc_caxecraft_content_FirstPlayableLevel_writeRun(struct hxc_caxecraft_domain_GameSession *hxc_session, int32_t hxc_start, int32_t hxc_code, int32_t hxc_count)
{
  if (!(hxc_start < 0))
  {
    if (hxc_session == NULL)
    {
      abort();
    }
    int32_t hxc_tmp_instance_call_result_n0 = hxc_caxecraft_domain_GameSession_writeTerrainRunDuringLoad(hxc_session, hxc_start, hxc_code, hxc_count);
    return hxc_tmp_instance_call_result_n0;
  }
  return -1;
}
