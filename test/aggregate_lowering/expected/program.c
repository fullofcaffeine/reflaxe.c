#include "hxc/program.h"

_Static_assert(offsetof(struct hxc_SwitchRecord, hxc_state) == 0, "closed record hxc_SwitchRecord first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_SwitchRecord) >= _Alignof(int32_t), "closed record hxc_SwitchRecord alignment admits field 0");

_Static_assert(sizeof(struct hxc_SwitchRecord) >= offsetof(struct hxc_SwitchRecord, hxc_state) + sizeof(int32_t), "closed record hxc_SwitchRecord size contains its final field");

_Static_assert(offsetof(struct hxc_OrderA, hxc_a) == 0, "closed record hxc_OrderA first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_OrderA) >= _Alignof(int32_t), "closed record hxc_OrderA alignment admits field 0");

_Static_assert(offsetof(struct hxc_OrderA, hxc_z) >= offsetof(struct hxc_OrderA, hxc_a) + sizeof(int32_t), "closed record hxc_OrderA field 1 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_OrderA) >= _Alignof(int32_t), "closed record hxc_OrderA alignment admits field 1");

_Static_assert(sizeof(struct hxc_OrderA) >= offsetof(struct hxc_OrderA, hxc_z) + sizeof(int32_t), "closed record hxc_OrderA size contains its final field");

_Static_assert(offsetof(struct hxc_Envelope, hxc_enabled) == 0, "closed record hxc_Envelope first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_Envelope) >= _Alignof(bool), "closed record hxc_Envelope alignment admits field 0");

_Static_assert(offsetof(struct hxc_Envelope, hxc_point) >= offsetof(struct hxc_Envelope, hxc_enabled) + sizeof(bool), "closed record hxc_Envelope field 1 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_Envelope) >= _Alignof(struct hxc_OrderA), "closed record hxc_Envelope alignment admits field 1");

_Static_assert(sizeof(struct hxc_Envelope) >= offsetof(struct hxc_Envelope, hxc_point) + sizeof(struct hxc_OrderA), "closed record hxc_Envelope size contains its final field");

struct hxc_OrderA hxc_AggregateFixture_copy(struct hxc_OrderA hxc_value)
{
  struct hxc_OrderA hxc_copied = hxc_value;
  return hxc_copied;
}

struct hxc_Envelope hxc_AggregateFixture_envelope(struct hxc_OrderA hxc_value)
{
  return (struct hxc_Envelope){ .hxc_enabled = true, .hxc_point = hxc_value };
}

int32_t hxc_AggregateFixture_envelopeSum(struct hxc_Envelope hxc_value)
{
  int32_t hxc_tmp_conditional_result_n1 = 0;
  if (hxc_value.hxc_enabled)
  {
    hxc_tmp_conditional_result_n1 = hxc_i32_add_wrapping(hxc_value.hxc_point.hxc_a, hxc_value.hxc_point.hxc_z);
  }
  else
  {
    hxc_tmp_conditional_result_n1 = 0;
  }
  return hxc_tmp_conditional_result_n1;
}

int32_t hxc_AggregateFixture_identity(int32_t hxc_value)
{
  return hxc_value;
}

int32_t hxc_AggregateFixture_localSum(int32_t hxc_left, int32_t hxc_right)
{
  struct hxc_OrderA hxc_tmp_call_result_n0 = hxc_AggregateFixture_make(hxc_left, hxc_right);
  struct hxc_OrderA hxc_value = hxc_tmp_call_result_n0;
  int32_t hxc_tmp_record_field_load_result_n1 = hxc_value.hxc_a;
  return hxc_i32_add_wrapping(hxc_tmp_record_field_load_result_n1, hxc_value.hxc_z);
}

void hxc_AggregateFixture_main(void)
{
  struct hxc_OrderA hxc_tmp_call_result_n0 = hxc_AggregateFixture_make(3, 4);
  struct hxc_OrderA hxc_first = hxc_tmp_call_result_n0;
  struct hxc_OrderA hxc_tmp_call_result_n2 = hxc_AggregateFixture_copy(hxc_first);
  struct hxc_OrderA hxc_copied = hxc_tmp_call_result_n2;
  struct hxc_Envelope hxc_tmp_call_result_n4 = hxc_AggregateFixture_envelope(hxc_copied);
  struct hxc_Envelope hxc_nested = hxc_tmp_call_result_n4;
  while (1)
  {
    int32_t hxc_tmp_call_result_n6 = hxc_AggregateFixture_sum(hxc_copied);
    bool hxc_tmp_short_circuit_result_n3 = hxc_tmp_call_result_n6 == 7;
    if (hxc_tmp_call_result_n6 == 7)
    {
      int32_t hxc_tmp_call_result_n7 = hxc_AggregateFixture_localSum(5, 6);
      hxc_tmp_short_circuit_result_n3 = hxc_tmp_call_result_n7 == 11;
    }
    bool hxc_tmp_short_circuit_load_result_n8 = hxc_tmp_short_circuit_result_n3;
    bool hxc_tmp_short_circuit_result_n4 = hxc_tmp_short_circuit_load_result_n8;
    if (hxc_tmp_short_circuit_load_result_n8)
    {
      int32_t hxc_tmp_call_result_n10 = hxc_AggregateFixture_envelopeSum(hxc_nested);
      hxc_tmp_short_circuit_result_n4 = hxc_tmp_call_result_n10 == 7;
    }
    bool hxc_tmp_short_circuit_load_result_n11 = hxc_tmp_short_circuit_result_n4;
    bool hxc_tmp_short_circuit_result_n5 = hxc_tmp_short_circuit_load_result_n11;
    if (hxc_tmp_short_circuit_load_result_n11)
    {
      int32_t hxc_tmp_call_result_n12 = hxc_AggregateFixture_switchStateValue(1);
      hxc_tmp_short_circuit_result_n5 = hxc_tmp_call_result_n12 == 7;
    }
    bool hxc_tmp_short_circuit_load_result_n13 = hxc_tmp_short_circuit_result_n5;
    bool hxc_tmp_short_circuit_result_n6 = hxc_tmp_short_circuit_load_result_n13;
    if (hxc_tmp_short_circuit_load_result_n13)
    {
      struct hxc_SwitchRecord hxc_tmp_call_result_n14 = hxc_AggregateFixture_makeSwitch(1);
      bool hxc_tmp_call_result_n15 = hxc_AggregateFixture_switchIsOn(hxc_tmp_call_result_n14);
      hxc_tmp_short_circuit_result_n6 = hxc_tmp_call_result_n15;
    }
    bool hxc_tmp_short_circuit_load_result_n16 = hxc_tmp_short_circuit_result_n6;
    bool hxc_tmp_short_circuit_result_n7 = hxc_tmp_short_circuit_load_result_n16;
    if (hxc_tmp_short_circuit_load_result_n16)
    {
      struct hxc_SwitchRecord hxc_tmp_call_result_n17 = hxc_AggregateFixture_makeSwitch(0);
      bool hxc_tmp_call_result_n18 = hxc_AggregateFixture_switchIsOn(hxc_tmp_call_result_n17);
      hxc_tmp_short_circuit_result_n7 = !hxc_tmp_call_result_n18;
    }
    if (!!hxc_tmp_short_circuit_result_n7)
    {
      break;
    }
  }
  return;
}

struct hxc_OrderA hxc_AggregateFixture_make(int32_t hxc_left, int32_t hxc_right)
{
  int32_t hxc_tmp_call_result_n0 = hxc_AggregateFixture_identity(hxc_left);
  int32_t hxc_tmp_call_result_n1 = hxc_AggregateFixture_identity(hxc_right);
  return (struct hxc_OrderA){ .hxc_a = hxc_tmp_call_result_n1, .hxc_z = hxc_tmp_call_result_n0 };
}

struct hxc_SwitchRecord hxc_AggregateFixture_makeSwitch(int32_t hxc_state)
{
  return (struct hxc_SwitchRecord){ .hxc_state = hxc_state };
}

int32_t hxc_AggregateFixture_sum(struct hxc_OrderA hxc_value)
{
  return hxc_i32_add_wrapping(hxc_value.hxc_a, hxc_value.hxc_z);
}

bool hxc_AggregateFixture_switchIsOn(struct hxc_SwitchRecord hxc_value)
{
  return hxc_value.hxc_state == 1;
}

int32_t hxc_AggregateFixture_switchStateValue(int32_t hxc_value)
{
  int32_t hxc_tmp_switch_result_n1 = 0;
  switch (hxc_value) {
    case 0:
      {
        hxc_tmp_switch_result_n1 = 3;
        break;
      }
    case 1:
      {
        hxc_tmp_switch_result_n1 = 7;
        break;
      }
    default:
      {
        hxc_tmp_switch_result_n1 = -1;
        break;
      }
  }
  return hxc_tmp_switch_result_n1;
}

int main(void)
{
  hxc_AggregateFixture_main();
  return 0;
}
