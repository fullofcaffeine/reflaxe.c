#include "hxc/program.h"

_Static_assert(hxc_Option_None_h00cd578bb80f == 0, "enum hxc_Option_ha0e4b5dcc139 case None retains its Haxe discriminant");

_Static_assert(hxc_Option_Some_h33493695ace2 == 1, "enum hxc_Option_ha0e4b5dcc139 case Some retains its Haxe discriminant");

_Static_assert(offsetof(struct hxc_Option_ha0e4b5dcc139, hxc_tag) == 0, "tagged enum hxc_Option_ha0e4b5dcc139 begins with its discriminant");

_Static_assert(offsetof(struct hxc_Option_ha0e4b5dcc139, hxc_payload) >= sizeof(enum hxc_Option_tag_h4f842caea9db), "tagged enum hxc_Option_ha0e4b5dcc139 payload follows its discriminant");

_Static_assert(sizeof(struct hxc_Option_ha0e4b5dcc139) >= offsetof(struct hxc_Option_ha0e4b5dcc139, hxc_payload) + sizeof(union hxc_Option_payload_ha68af457b79c), "tagged enum hxc_Option_ha0e4b5dcc139 contains its payload union");

_Static_assert(offsetof(union hxc_Option_payload_ha68af457b79c, hxc_Some) == 0, "tagged enum hxc_Option_ha0e4b5dcc139 case Some begins at union offset zero");

_Static_assert(offsetof(struct hxc_Option_Some_payload_hc2b8e9073c59, hxc_value) == 0, "tagged enum hxc_Option_ha0e4b5dcc139 case Some first payload begins at zero");

_Static_assert(_Alignof(struct hxc_Option_Some_payload_hc2b8e9073c59) >= _Alignof(bool), "tagged enum hxc_Option_ha0e4b5dcc139 case Some admits payload 0 alignment");

_Static_assert(hxc_Option_None_h506b5e6013bd == 0, "enum hxc_Option_h95f1c4a28dac case None retains its Haxe discriminant");

_Static_assert(hxc_Option_Some_ha9454146ff01 == 1, "enum hxc_Option_h95f1c4a28dac case Some retains its Haxe discriminant");

_Static_assert(offsetof(struct hxc_Option_h95f1c4a28dac, hxc_tag) == 0, "tagged enum hxc_Option_h95f1c4a28dac begins with its discriminant");

_Static_assert(offsetof(struct hxc_Option_h95f1c4a28dac, hxc_payload) >= sizeof(enum hxc_Option_tag_h51b3904815c1), "tagged enum hxc_Option_h95f1c4a28dac payload follows its discriminant");

_Static_assert(sizeof(struct hxc_Option_h95f1c4a28dac) >= offsetof(struct hxc_Option_h95f1c4a28dac, hxc_payload) + sizeof(union hxc_Option_payload_h331368fdb4fc), "tagged enum hxc_Option_h95f1c4a28dac contains its payload union");

_Static_assert(offsetof(union hxc_Option_payload_h331368fdb4fc, hxc_Some) == 0, "tagged enum hxc_Option_h95f1c4a28dac case Some begins at union offset zero");

_Static_assert(offsetof(struct hxc_Option_Some_payload_h6fa8fca385dc, hxc_value) == 0, "tagged enum hxc_Option_h95f1c4a28dac case Some first payload begins at zero");

_Static_assert(_Alignof(struct hxc_Option_Some_payload_h6fa8fca385dc) >= _Alignof(int32_t), "tagged enum hxc_Option_h95f1c4a28dac case Some admits payload 0 alignment");

_Static_assert(hxc_Chain_End == 0, "enum hxc_Chain case End retains its Haxe discriminant");

_Static_assert(hxc_Chain_Link == 1, "enum hxc_Chain case Link retains its Haxe discriminant");

_Static_assert(offsetof(struct hxc_Chain, hxc_tag) == 0, "tagged enum hxc_Chain begins with its discriminant");

_Static_assert(offsetof(struct hxc_Chain, hxc_payload) >= sizeof(enum hxc_Chain_tag), "tagged enum hxc_Chain payload follows its discriminant");

_Static_assert(sizeof(struct hxc_Chain) >= offsetof(struct hxc_Chain, hxc_payload) + sizeof(union hxc_Chain_payload), "tagged enum hxc_Chain contains its payload union");

_Static_assert(offsetof(union hxc_Chain_payload, hxc_End) == 0, "tagged enum hxc_Chain case End begins at union offset zero");

_Static_assert(offsetof(struct hxc_Chain_End_payload, hxc_value) == 0, "tagged enum hxc_Chain case End first payload begins at zero");

_Static_assert(_Alignof(struct hxc_Chain_End_payload) >= _Alignof(int32_t), "tagged enum hxc_Chain case End admits payload 0 alignment");

_Static_assert(offsetof(union hxc_Chain_payload, hxc_Link) == 0, "tagged enum hxc_Chain case Link begins at union offset zero");

_Static_assert(offsetof(struct hxc_Chain_Link_payload, hxc_value) == 0, "tagged enum hxc_Chain case Link first payload begins at zero");

_Static_assert(_Alignof(struct hxc_Chain_Link_payload) >= _Alignof(int32_t), "tagged enum hxc_Chain case Link admits payload 0 alignment");

_Static_assert(offsetof(struct hxc_Chain_Link_payload, hxc_next) >= offsetof(struct hxc_Chain_Link_payload, hxc_value) + sizeof(int32_t), "tagged enum hxc_Chain case Link payload 1 follows its predecessor");

_Static_assert(_Alignof(struct hxc_Chain_Link_payload) >= _Alignof(struct hxc_Chain *), "tagged enum hxc_Chain case Link admits payload 1 alignment");

_Static_assert(hxc_Mode_Off == 0, "enum hxc_Mode case Off retains its Haxe discriminant");

_Static_assert(hxc_Mode_On == 1, "enum hxc_Mode case On retains its Haxe discriminant");

int32_t hxc_EnumFixture_boolOptionValue(struct hxc_Option_ha0e4b5dcc139 hxc_value_hb6e6538779c8)
{
  int32_t hxc_tmp_enum_switch_result_n1 = 0;
  switch (hxc_value_hb6e6538779c8.hxc_tag) {
    case hxc_Option_None_h00cd578bb80f:
      {
        hxc_tmp_enum_switch_result_n1 = -1;
        break;
      }
    case hxc_Option_Some_h33493695ace2:
      {
        if (hxc_value_hb6e6538779c8.hxc_tag != hxc_Option_Some_h33493695ace2)
        {
          abort();
        }
        bool hxc_tmp_enum_payload_project_n0 = hxc_value_hb6e6538779c8.hxc_payload.hxc_Some.hxc_value;
        bool hxc_value_h85a49fca2ea0 = hxc_tmp_enum_payload_project_n0;
        bool hxc_payload = hxc_value_h85a49fca2ea0;
        bool hxc_tmp_load_result_n2 = hxc_payload;
        int32_t hxc_tmp_conditional_result_n4 = 0;
        if (hxc_tmp_load_result_n2)
        {
          hxc_tmp_conditional_result_n4 = 1;
        }
        else
        {
          hxc_tmp_conditional_result_n4 = 0;
        }
        hxc_tmp_enum_switch_result_n1 = hxc_tmp_conditional_result_n4;
        break;
      }
  }
  return hxc_tmp_enum_switch_result_n1;
}

int32_t hxc_EnumFixture_guardedValue(struct hxc_Option_h95f1c4a28dac hxc_value_ha201421511a7)
{
  int32_t hxc_tmp_enum_switch_result_n1 = 0;
  switch (hxc_value_ha201421511a7.hxc_tag) {
    case hxc_Option_None_h506b5e6013bd:
      {
        hxc_tmp_enum_switch_result_n1 = -1;
        break;
      }
    case hxc_Option_Some_ha9454146ff01:
      {
        if (hxc_value_ha201421511a7.hxc_tag != hxc_Option_Some_ha9454146ff01)
        {
          abort();
        }
        int32_t hxc_tmp_enum_payload_project_n0 = hxc_value_ha201421511a7.hxc_payload.hxc_Some.hxc_value;
        int32_t hxc_value_h8792d0ecd498 = hxc_tmp_enum_payload_project_n0;
        int32_t hxc_payload_h6e5ec5c1b4a8 = hxc_value_h8792d0ecd498;
        int32_t hxc_tmp_load_result_n2 = hxc_payload_h6e5ec5c1b4a8;
        int32_t hxc_tmp_conditional_result_n4 = 0;
        if (hxc_tmp_load_result_n2 > 4)
        {
          hxc_tmp_conditional_result_n4 = hxc_payload_h6e5ec5c1b4a8;
        }
        else
        {
          int32_t hxc_payload_h0b6a49bc2290 = hxc_value_h8792d0ecd498;
          hxc_tmp_conditional_result_n4 = hxc_i32_add_wrapping(hxc_payload_h0b6a49bc2290, 1);
        }
        hxc_tmp_enum_switch_result_n1 = hxc_tmp_conditional_result_n4;
        break;
      }
  }
  return hxc_tmp_enum_switch_result_n1;
}

int32_t hxc_EnumFixture_identity(int32_t hxc_value)
{
  return hxc_value;
}

void hxc_EnumFixture_main(void)
{
  enum hxc_Mode hxc_mode = hxc_Mode_On;
  int32_t hxc_tmp_call_result_n1 = hxc_EnumFixture_identity(7);
  struct hxc_Option_h95f1c4a28dac hxc_present = (struct hxc_Option_h95f1c4a28dac){ .hxc_tag = hxc_Option_Some_ha9454146ff01, .hxc_payload.hxc_Some.hxc_value = hxc_tmp_call_result_n1 };
  struct hxc_Option_h95f1c4a28dac hxc_absent = (struct hxc_Option_h95f1c4a28dac){ .hxc_tag = hxc_Option_None_h506b5e6013bd };
  struct hxc_Option_ha0e4b5dcc139 hxc_truth = (struct hxc_Option_ha0e4b5dcc139){ .hxc_tag = hxc_Option_Some_h33493695ace2, .hxc_payload.hxc_Some.hxc_value = true };
  while (1)
  {
    int32_t hxc_tmp_call_result_n6 = hxc_EnumFixture_modeValue(hxc_mode);
    bool hxc_tmp_short_circuit_result_n4 = hxc_tmp_call_result_n6 == 1;
    if (hxc_tmp_call_result_n6 == 1)
    {
      int32_t hxc_tmp_call_result_n8 = hxc_EnumFixture_optionValue(hxc_present);
      hxc_tmp_short_circuit_result_n4 = hxc_tmp_call_result_n8 == 7;
    }
    bool hxc_tmp_short_circuit_load_result_n9 = hxc_tmp_short_circuit_result_n4;
    bool hxc_tmp_short_circuit_result_n5 = hxc_tmp_short_circuit_load_result_n9;
    if (hxc_tmp_short_circuit_load_result_n9)
    {
      int32_t hxc_tmp_call_result_n11 = hxc_EnumFixture_optionValue(hxc_absent);
      hxc_tmp_short_circuit_result_n5 = hxc_tmp_call_result_n11 == 0;
    }
    bool hxc_tmp_short_circuit_load_result_n12 = hxc_tmp_short_circuit_result_n5;
    bool hxc_tmp_short_circuit_result_n6 = hxc_tmp_short_circuit_load_result_n12;
    if (hxc_tmp_short_circuit_load_result_n12)
    {
      int32_t hxc_tmp_call_result_n14 = hxc_EnumFixture_guardedValue(hxc_present);
      hxc_tmp_short_circuit_result_n6 = hxc_tmp_call_result_n14 == 7;
    }
    bool hxc_tmp_short_circuit_load_result_n15 = hxc_tmp_short_circuit_result_n6;
    bool hxc_tmp_short_circuit_result_n7 = hxc_tmp_short_circuit_load_result_n15;
    if (hxc_tmp_short_circuit_load_result_n15)
    {
      int32_t hxc_tmp_call_result_n17 = hxc_EnumFixture_boolOptionValue(hxc_truth);
      hxc_tmp_short_circuit_result_n7 = hxc_tmp_call_result_n17 == 1;
    }
    bool hxc_tmp_short_circuit_load_result_n18 = hxc_tmp_short_circuit_result_n7;
    bool hxc_tmp_short_circuit_result_n8 = hxc_tmp_short_circuit_load_result_n18;
    if (hxc_tmp_short_circuit_load_result_n18)
    {
      int32_t hxc_tmp_call_result_n19 = hxc_EnumFixture_recursiveLocal();
      hxc_tmp_short_circuit_result_n8 = hxc_tmp_call_result_n19 == 3;
    }
    if (!!hxc_tmp_short_circuit_result_n8)
    {
      break;
    }
  }
  return;
}

int32_t hxc_EnumFixture_modeValue(enum hxc_Mode hxc_value)
{
  int32_t hxc_tmp_enum_switch_result_n1 = 0;
  switch (hxc_value) {
    case hxc_Mode_Off:
      {
        hxc_tmp_enum_switch_result_n1 = 0;
        break;
      }
    case hxc_Mode_On:
      {
        hxc_tmp_enum_switch_result_n1 = 1;
        break;
      }
  }
  return hxc_tmp_enum_switch_result_n1;
}

int32_t hxc_EnumFixture_optionValue(struct hxc_Option_h95f1c4a28dac hxc_value_h2c5c76013588)
{
  int32_t hxc_tmp_enum_switch_result_n1 = 0;
  switch (hxc_value_h2c5c76013588.hxc_tag) {
    case hxc_Option_None_h506b5e6013bd:
      {
        hxc_tmp_enum_switch_result_n1 = 0;
        break;
      }
    case hxc_Option_Some_ha9454146ff01:
      {
        if (hxc_value_h2c5c76013588.hxc_tag != hxc_Option_Some_ha9454146ff01)
        {
          abort();
        }
        int32_t hxc_tmp_enum_payload_project_n0 = hxc_value_h2c5c76013588.hxc_payload.hxc_Some.hxc_value;
        int32_t hxc_value_h37075a18294f = hxc_tmp_enum_payload_project_n0;
        int32_t hxc_payload = hxc_value_h37075a18294f;
        hxc_tmp_enum_switch_result_n1 = hxc_payload;
        break;
      }
  }
  return hxc_tmp_enum_switch_result_n1;
}

int32_t hxc_EnumFixture_recursiveLocal(void)
{
  struct hxc_Chain hxc_tail = (struct hxc_Chain){ .hxc_tag = hxc_Chain_End, .hxc_payload.hxc_End.hxc_value = 2 };
  struct hxc_Chain hxc_tmp_enum_recursive_payload_1_n2 = hxc_tail;
  struct hxc_Chain *hxc_tmp_enum_recursive_payload_address_n2 = &hxc_tmp_enum_recursive_payload_1_n2;
  struct hxc_Chain hxc_head = (struct hxc_Chain){ .hxc_tag = hxc_Chain_Link, .hxc_payload.hxc_Link.hxc_value = 1, .hxc_payload.hxc_Link.hxc_next = hxc_tmp_enum_recursive_payload_address_n2 };
  struct hxc_Chain hxc_tmp_load_result_n4 = hxc_head;
  int32_t hxc_tmp_enum_switch_result_n3 = 0;
  switch (hxc_tmp_load_result_n4.hxc_tag) {
    case hxc_Chain_End:
      {
        if (hxc_head.hxc_tag != hxc_Chain_End)
        {
          abort();
        }
        int32_t hxc_tmp_enum_payload_project_n6 = hxc_head.hxc_payload.hxc_End.hxc_value;
        int32_t hxc_value_h34f86739fe15 = hxc_tmp_enum_payload_project_n6;
        int32_t hxc_value_hc78beb4810c4 = hxc_value_h34f86739fe15;
        hxc_tmp_enum_switch_result_n3 = hxc_value_hc78beb4810c4;
        break;
      }
    case hxc_Chain_Link:
      {
        if (hxc_head.hxc_tag != hxc_Chain_Link)
        {
          abort();
        }
        int32_t hxc_tmp_enum_payload_project_n10 = hxc_head.hxc_payload.hxc_Link.hxc_value;
        int32_t hxc_value_h279cf5a91399 = hxc_tmp_enum_payload_project_n10;
        if (hxc_head.hxc_tag != hxc_Chain_Link)
        {
          abort();
        }
        struct hxc_Chain *hxc_tmp_enum_payload_project_n12 = hxc_head.hxc_payload.hxc_Link.hxc_next;
        struct hxc_Chain hxc_tmp_enum_recursive_payload_load_result_n13 = *hxc_tmp_enum_payload_project_n12;
        struct hxc_Chain hxc_next_h07fb61766894 = hxc_tmp_enum_recursive_payload_load_result_n13;
        int32_t hxc_value_hba17dd6c799c = hxc_value_h279cf5a91399;
        struct hxc_Chain hxc_next_he440e694a9fe = hxc_next_h07fb61766894;
        struct hxc_Chain hxc_tmp_load_result_n16 = hxc_next_he440e694a9fe;
        int32_t hxc_tmp_enum_switch_result_n10 = 0;
        switch (hxc_tmp_load_result_n16.hxc_tag) {
          case hxc_Chain_End:
            {
              if (hxc_next_he440e694a9fe.hxc_tag != hxc_Chain_End)
              {
                abort();
              }
              int32_t hxc_tmp_enum_payload_project_n18 = hxc_next_he440e694a9fe.hxc_payload.hxc_End.hxc_value;
              int32_t hxc_value_h443e3a509706 = hxc_tmp_enum_payload_project_n18;
              int32_t hxc_last = hxc_value_h443e3a509706;
              int32_t hxc_tmp_load_result_n20 = hxc_value_hba17dd6c799c;
              hxc_tmp_enum_switch_result_n10 = hxc_i32_add_wrapping(hxc_tmp_load_result_n20, hxc_last);
              break;
            }
          case hxc_Chain_Link:
            {
              hxc_tmp_enum_switch_result_n10 = 0;
              break;
            }
        }
        hxc_tmp_enum_switch_result_n3 = hxc_tmp_enum_switch_result_n10;
        break;
      }
  }
  return hxc_tmp_enum_switch_result_n3;
}

int main(void)
{
  hxc_EnumFixture_main();
  return 0;
}
