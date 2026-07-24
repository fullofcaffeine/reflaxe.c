#include "hxc/program.h"

_Static_assert(offsetof(struct hxc_OptionalEnvelope, hxc_point) == 0, "closed record hxc_OptionalEnvelope first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_OptionalEnvelope) >= _Alignof(struct hxc_optional_OrderA), "closed record hxc_OptionalEnvelope alignment admits field 0");

_Static_assert(sizeof(struct hxc_OptionalEnvelope) >= offsetof(struct hxc_OptionalEnvelope, hxc_point) + sizeof(struct hxc_optional_OrderA), "closed record hxc_OptionalEnvelope size contains its final field");

_Static_assert(offsetof(struct hxc_SwitchRecord, hxc_state) == 0, "closed record hxc_SwitchRecord first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_SwitchRecord) >= _Alignof(int32_t), "closed record hxc_SwitchRecord alignment admits field 0");

_Static_assert(sizeof(struct hxc_SwitchRecord) >= offsetof(struct hxc_SwitchRecord, hxc_state) + sizeof(int32_t), "closed record hxc_SwitchRecord size contains its final field");

_Static_assert(offsetof(struct hxc_OrderA, hxc_a) == 0, "closed record hxc_OrderA first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_OrderA) >= _Alignof(int32_t), "closed record hxc_OrderA alignment admits field 0");

_Static_assert(offsetof(struct hxc_OrderA, hxc_z) >= offsetof(struct hxc_OrderA, hxc_a) + sizeof(int32_t), "closed record hxc_OrderA field 1 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_OrderA) >= _Alignof(int32_t), "closed record hxc_OrderA alignment admits field 1");

_Static_assert(sizeof(struct hxc_OrderA) >= offsetof(struct hxc_OrderA, hxc_z) + sizeof(int32_t), "closed record hxc_OrderA size contains its final field");

_Static_assert(offsetof(struct hxc_ConditionalRecord, hxc_order) == 0, "closed record hxc_ConditionalRecord first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_ConditionalRecord) >= _Alignof(int32_t), "closed record hxc_ConditionalRecord alignment admits field 0");

_Static_assert(offsetof(struct hxc_ConditionalRecord, hxc_value) >= offsetof(struct hxc_ConditionalRecord, hxc_order) + sizeof(int32_t), "closed record hxc_ConditionalRecord field 1 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_ConditionalRecord) >= _Alignof(struct hxc_OrderA), "closed record hxc_ConditionalRecord alignment admits field 1");

_Static_assert(sizeof(struct hxc_ConditionalRecord) >= offsetof(struct hxc_ConditionalRecord, hxc_value) + sizeof(struct hxc_OrderA), "closed record hxc_ConditionalRecord size contains its final field");

_Static_assert(offsetof(struct hxc_FlowRecord, hxc_first) == 0, "closed record hxc_FlowRecord first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_FlowRecord) >= _Alignof(int32_t), "closed record hxc_FlowRecord alignment admits field 0");

_Static_assert(offsetof(struct hxc_FlowRecord, hxc_order) >= offsetof(struct hxc_FlowRecord, hxc_first) + sizeof(int32_t), "closed record hxc_FlowRecord field 1 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_FlowRecord) >= _Alignof(int32_t), "closed record hxc_FlowRecord alignment admits field 1");

_Static_assert(offsetof(struct hxc_FlowRecord, hxc_second) >= offsetof(struct hxc_FlowRecord, hxc_order) + sizeof(int32_t), "closed record hxc_FlowRecord field 2 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_FlowRecord) >= _Alignof(int32_t), "closed record hxc_FlowRecord alignment admits field 2");

_Static_assert(offsetof(struct hxc_FlowRecord, hxc_third) >= offsetof(struct hxc_FlowRecord, hxc_second) + sizeof(int32_t), "closed record hxc_FlowRecord field 3 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_FlowRecord) >= _Alignof(int32_t), "closed record hxc_FlowRecord alignment admits field 3");

_Static_assert(sizeof(struct hxc_FlowRecord) >= offsetof(struct hxc_FlowRecord, hxc_third) + sizeof(int32_t), "closed record hxc_FlowRecord size contains its final field");

_Static_assert(offsetof(struct hxc_Envelope, hxc_enabled) == 0, "closed record hxc_Envelope first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_Envelope) >= _Alignof(bool), "closed record hxc_Envelope alignment admits field 0");

_Static_assert(offsetof(struct hxc_Envelope, hxc_point) >= offsetof(struct hxc_Envelope, hxc_enabled) + sizeof(bool), "closed record hxc_Envelope field 1 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_Envelope) >= _Alignof(struct hxc_OrderA), "closed record hxc_Envelope alignment admits field 1");

_Static_assert(sizeof(struct hxc_Envelope) >= offsetof(struct hxc_Envelope, hxc_point) + sizeof(struct hxc_OrderA), "closed record hxc_Envelope size contains its final field");

_Static_assert(offsetof(struct hxc_ActorRecord, hxc_phase) == 0, "closed record hxc_ActorRecord first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_ActorRecord) >= _Alignof(struct hxc_ActorPhase), "closed record hxc_ActorRecord alignment admits field 0");

_Static_assert(sizeof(struct hxc_ActorRecord) >= offsetof(struct hxc_ActorRecord, hxc_phase) + sizeof(struct hxc_ActorPhase), "closed record hxc_ActorRecord size contains its final field");

_Static_assert(hxc_ActorPhase_Waiting == 0, "enum hxc_ActorPhase case Waiting retains its Haxe discriminant");

_Static_assert(hxc_ActorPhase_Moving == 1, "enum hxc_ActorPhase case Moving retains its Haxe discriminant");

_Static_assert(offsetof(struct hxc_ActorPhase, hxc_tag) == 0, "tagged enum hxc_ActorPhase begins with its discriminant");

_Static_assert(offsetof(struct hxc_ActorPhase, hxc_payload) >= sizeof(enum hxc_ActorPhase_tag), "tagged enum hxc_ActorPhase payload follows its discriminant");

_Static_assert(sizeof(struct hxc_ActorPhase) >= offsetof(struct hxc_ActorPhase, hxc_payload) + sizeof(union hxc_ActorPhase_payload), "tagged enum hxc_ActorPhase contains its payload union");

_Static_assert(offsetof(union hxc_ActorPhase_payload, hxc_Moving) == 0, "tagged enum hxc_ActorPhase case Moving begins at union offset zero");

_Static_assert(offsetof(struct hxc_ActorPhase_Moving_payload, hxc_speed) == 0, "tagged enum hxc_ActorPhase case Moving first payload begins at zero");

_Static_assert(_Alignof(struct hxc_ActorPhase_Moving_payload) >= _Alignof(int32_t), "tagged enum hxc_ActorPhase case Moving admits payload 0 alignment");

int32_t hxc_AggregateFixture_actorSpeed(struct hxc_ActorRecord hxc_value)
{
  struct hxc_ActorPhase hxc_symbol = hxc_value.hxc_phase;
  struct hxc_ActorPhase hxc_tmp_load_result_n1 = hxc_symbol;
  int32_t hxc_tmp_enum_switch_result_n2 = 0;
  switch (hxc_tmp_load_result_n1.hxc_tag) {
    case hxc_ActorPhase_Waiting:
      {
        hxc_tmp_enum_switch_result_n2 = 0;
        break;
      }
    case hxc_ActorPhase_Moving:
      {
        if (hxc_symbol.hxc_tag != hxc_ActorPhase_Moving)
        {
          abort();
        }
        int32_t hxc_tmp_enum_payload_project_n3 = hxc_symbol.hxc_payload.hxc_Moving.hxc_speed;
        int32_t hxc_speed_h8bb51c0b9c04 = hxc_tmp_enum_payload_project_n3;
        int32_t hxc_speed_h577db6321a50 = hxc_speed_h8bb51c0b9c04;
        hxc_tmp_enum_switch_result_n2 = hxc_speed_h577db6321a50;
        break;
      }
  }
  return hxc_tmp_enum_switch_result_n2;
}

int32_t hxc_AggregateFixture_checkedField(int32_t hxc_value)
{
  int32_t hxc_tmp_conditional_result_n1 = 0;
  if (hxc_value < 0)
  {
    hxc_tmp_conditional_result_n1 = hxc_i32_negate_wrapping(hxc_value);
  }
  else
  {
    hxc_tmp_conditional_result_n1 = hxc_value;
  }
  return hxc_tmp_conditional_result_n1;
}

int32_t hxc_AggregateFixture_conditionalRecordSum(bool hxc_chooseFirst, struct hxc_OrderA hxc_first, struct hxc_OrderA hxc_second)
{
  struct hxc_OrderA hxc_tmp_conditional_result_n3;
  if (hxc_chooseFirst)
  {
    hxc_tmp_conditional_result_n3 = hxc_first;
  }
  else
  {
    hxc_tmp_conditional_result_n3 = hxc_second;
  }
  int32_t hxc_tmp_call_result_n1 = hxc_AggregateFixture_sum(hxc_tmp_conditional_result_n3);
  return hxc_tmp_call_result_n1;
}

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
  struct hxc_optional_OrderA hxc_tmp_call_result_n5 = hxc_AggregateFixture_noPoint();
  struct hxc_OptionalEnvelope hxc_tmp_call_result_n6 = hxc_AggregateFixture_optionalEnvelope(hxc_tmp_call_result_n5);
  struct hxc_OptionalEnvelope hxc_absent = hxc_tmp_call_result_n6;
  struct hxc_optional_OrderA hxc_tmp_call_result_n8 = hxc_AggregateFixture_somePoint(hxc_copied);
  struct hxc_OptionalEnvelope hxc_tmp_call_result_n9 = hxc_AggregateFixture_optionalEnvelope(hxc_tmp_call_result_n8);
  struct hxc_OptionalEnvelope hxc_present = hxc_tmp_call_result_n9;
  struct hxc_FlowRecord hxc_tmp_call_result_n10 = hxc_AggregateFixture_makeFlowRecord(-8, 9, -10);
  struct hxc_FlowRecord hxc_flow = hxc_tmp_call_result_n10;
  struct hxc_ConditionalRecord hxc_tmp_call_result_n13 = hxc_AggregateFixture_selectRecord(true, hxc_first, (struct hxc_OrderA){ .hxc_a = 40, .hxc_z = 30 });
  struct hxc_ConditionalRecord hxc_selectedFirst = hxc_tmp_call_result_n13;
  struct hxc_ConditionalRecord hxc_tmp_call_result_n16 = hxc_AggregateFixture_selectRecord(false, hxc_first, (struct hxc_OrderA){ .hxc_a = 40, .hxc_z = 30 });
  struct hxc_ConditionalRecord hxc_selectedSecond = hxc_tmp_call_result_n16;
  while (1)
  {
    int32_t hxc_tmp_call_result_n18 = hxc_AggregateFixture_sum(hxc_copied);
    bool hxc_tmp_short_circuit_result_n8 = hxc_tmp_call_result_n18 == 7;
    if (hxc_tmp_call_result_n18 == 7)
    {
      int32_t hxc_tmp_call_result_n19 = hxc_AggregateFixture_localSum(5, 6);
      hxc_tmp_short_circuit_result_n8 = hxc_tmp_call_result_n19 == 11;
    }
    bool hxc_tmp_short_circuit_load_result_n20 = hxc_tmp_short_circuit_result_n8;
    bool hxc_tmp_short_circuit_result_n9 = hxc_tmp_short_circuit_load_result_n20;
    if (hxc_tmp_short_circuit_load_result_n20)
    {
      int32_t hxc_tmp_call_result_n22 = hxc_AggregateFixture_envelopeSum(hxc_nested);
      hxc_tmp_short_circuit_result_n9 = hxc_tmp_call_result_n22 == 7;
    }
    bool hxc_tmp_short_circuit_load_result_n23 = hxc_tmp_short_circuit_result_n9;
    bool hxc_tmp_short_circuit_result_n10 = hxc_tmp_short_circuit_load_result_n23;
    if (hxc_tmp_short_circuit_load_result_n23)
    {
      hxc_tmp_short_circuit_result_n10 = !hxc_absent.hxc_point.hxc_has_value;
    }
    bool hxc_tmp_short_circuit_load_result_n25 = hxc_tmp_short_circuit_result_n10;
    bool hxc_tmp_short_circuit_result_n11 = hxc_tmp_short_circuit_load_result_n25;
    if (hxc_tmp_short_circuit_load_result_n25)
    {
      hxc_tmp_short_circuit_result_n11 = hxc_present.hxc_point.hxc_has_value;
    }
    bool hxc_tmp_short_circuit_load_result_n27 = hxc_tmp_short_circuit_result_n11;
    bool hxc_tmp_short_circuit_result_n12 = hxc_tmp_short_circuit_load_result_n27;
    if (hxc_tmp_short_circuit_load_result_n27)
    {
      int32_t hxc_tmp_call_result_n29 = hxc_AggregateFixture_optionalSum(hxc_absent.hxc_point);
      hxc_tmp_short_circuit_result_n12 = hxc_tmp_call_result_n29 == 0;
    }
    bool hxc_tmp_short_circuit_load_result_n30 = hxc_tmp_short_circuit_result_n12;
    bool hxc_tmp_short_circuit_result_n13 = hxc_tmp_short_circuit_load_result_n30;
    if (hxc_tmp_short_circuit_load_result_n30)
    {
      int32_t hxc_tmp_call_result_n32 = hxc_AggregateFixture_optionalSum(hxc_present.hxc_point);
      hxc_tmp_short_circuit_result_n13 = hxc_tmp_call_result_n32 == 7;
    }
    bool hxc_tmp_short_circuit_load_result_n33 = hxc_tmp_short_circuit_result_n13;
    bool hxc_tmp_short_circuit_result_n14 = hxc_tmp_short_circuit_load_result_n33;
    if (hxc_tmp_short_circuit_load_result_n33)
    {
      int32_t hxc_tmp_call_result_n34 = hxc_AggregateFixture_switchStateValue(1);
      hxc_tmp_short_circuit_result_n14 = hxc_tmp_call_result_n34 == 7;
    }
    bool hxc_tmp_short_circuit_load_result_n35 = hxc_tmp_short_circuit_result_n14;
    bool hxc_tmp_short_circuit_result_n15 = hxc_tmp_short_circuit_load_result_n35;
    if (hxc_tmp_short_circuit_load_result_n35)
    {
      struct hxc_SwitchRecord hxc_tmp_call_result_n36 = hxc_AggregateFixture_makeSwitch(1);
      bool hxc_tmp_call_result_n37 = hxc_AggregateFixture_switchIsOn(hxc_tmp_call_result_n36);
      hxc_tmp_short_circuit_result_n15 = hxc_tmp_call_result_n37;
    }
    bool hxc_tmp_short_circuit_load_result_n38 = hxc_tmp_short_circuit_result_n15;
    bool hxc_tmp_short_circuit_result_n16 = hxc_tmp_short_circuit_load_result_n38;
    if (hxc_tmp_short_circuit_load_result_n38)
    {
      struct hxc_SwitchRecord hxc_tmp_call_result_n39 = hxc_AggregateFixture_makeSwitch(0);
      bool hxc_tmp_call_result_n40 = hxc_AggregateFixture_switchIsOn(hxc_tmp_call_result_n39);
      hxc_tmp_short_circuit_result_n16 = !hxc_tmp_call_result_n40;
    }
    bool hxc_tmp_short_circuit_load_result_n41 = hxc_tmp_short_circuit_result_n16;
    bool hxc_tmp_short_circuit_result_n17 = hxc_tmp_short_circuit_load_result_n41;
    if (hxc_tmp_short_circuit_load_result_n41)
    {
      struct hxc_ActorRecord hxc_tmp_call_result_n43 = hxc_AggregateFixture_makeActor((struct hxc_ActorPhase){ .hxc_tag = hxc_ActorPhase_Moving, .hxc_payload.hxc_Moving.hxc_speed = 5 });
      int32_t hxc_tmp_call_result_n44 = hxc_AggregateFixture_actorSpeed(hxc_tmp_call_result_n43);
      hxc_tmp_short_circuit_result_n17 = hxc_tmp_call_result_n44 == 5;
    }
    bool hxc_tmp_short_circuit_load_result_n45 = hxc_tmp_short_circuit_result_n17;
    bool hxc_tmp_short_circuit_result_n18 = hxc_tmp_short_circuit_load_result_n45;
    if (hxc_tmp_short_circuit_load_result_n45)
    {
      struct hxc_ActorRecord hxc_tmp_call_result_n47 = hxc_AggregateFixture_makeActor((struct hxc_ActorPhase){ .hxc_tag = hxc_ActorPhase_Waiting });
      int32_t hxc_tmp_call_result_n48 = hxc_AggregateFixture_actorSpeed(hxc_tmp_call_result_n47);
      hxc_tmp_short_circuit_result_n18 = hxc_tmp_call_result_n48 == 0;
    }
    bool hxc_tmp_short_circuit_load_result_n49 = hxc_tmp_short_circuit_result_n18;
    bool hxc_tmp_short_circuit_result_n19 = hxc_tmp_short_circuit_load_result_n49;
    if (hxc_tmp_short_circuit_load_result_n49)
    {
      hxc_tmp_short_circuit_result_n19 = hxc_flow.hxc_first == 8;
    }
    bool hxc_tmp_short_circuit_load_result_n51 = hxc_tmp_short_circuit_result_n19;
    bool hxc_tmp_short_circuit_result_n20 = hxc_tmp_short_circuit_load_result_n51;
    if (hxc_tmp_short_circuit_load_result_n51)
    {
      hxc_tmp_short_circuit_result_n20 = hxc_flow.hxc_second == 9;
    }
    bool hxc_tmp_short_circuit_load_result_n53 = hxc_tmp_short_circuit_result_n20;
    bool hxc_tmp_short_circuit_result_n21 = hxc_tmp_short_circuit_load_result_n53;
    if (hxc_tmp_short_circuit_load_result_n53)
    {
      hxc_tmp_short_circuit_result_n21 = hxc_flow.hxc_third == 10;
    }
    bool hxc_tmp_short_circuit_load_result_n55 = hxc_tmp_short_circuit_result_n21;
    bool hxc_tmp_short_circuit_result_n22 = hxc_tmp_short_circuit_load_result_n55;
    if (hxc_tmp_short_circuit_load_result_n55)
    {
      hxc_tmp_short_circuit_result_n22 = hxc_flow.hxc_order == 123;
    }
    bool hxc_tmp_short_circuit_load_result_n57 = hxc_tmp_short_circuit_result_n22;
    bool hxc_tmp_short_circuit_result_n23 = hxc_tmp_short_circuit_load_result_n57;
    if (hxc_tmp_short_circuit_load_result_n57)
    {
      hxc_tmp_short_circuit_result_n23 = hxc_selectedFirst.hxc_value.hxc_z == 3;
    }
    bool hxc_tmp_short_circuit_load_result_n59 = hxc_tmp_short_circuit_result_n23;
    bool hxc_tmp_short_circuit_result_n24 = hxc_tmp_short_circuit_load_result_n59;
    if (hxc_tmp_short_circuit_load_result_n59)
    {
      hxc_tmp_short_circuit_result_n24 = hxc_selectedFirst.hxc_value.hxc_a == 4;
    }
    bool hxc_tmp_short_circuit_load_result_n61 = hxc_tmp_short_circuit_result_n24;
    bool hxc_tmp_short_circuit_result_n25 = hxc_tmp_short_circuit_load_result_n61;
    if (hxc_tmp_short_circuit_load_result_n61)
    {
      hxc_tmp_short_circuit_result_n25 = hxc_selectedFirst.hxc_order == 1;
    }
    bool hxc_tmp_short_circuit_load_result_n63 = hxc_tmp_short_circuit_result_n25;
    bool hxc_tmp_short_circuit_result_n26 = hxc_tmp_short_circuit_load_result_n63;
    if (hxc_tmp_short_circuit_load_result_n63)
    {
      hxc_tmp_short_circuit_result_n26 = hxc_selectedSecond.hxc_value.hxc_z == 30;
    }
    bool hxc_tmp_short_circuit_load_result_n65 = hxc_tmp_short_circuit_result_n26;
    bool hxc_tmp_short_circuit_result_n27 = hxc_tmp_short_circuit_load_result_n65;
    if (hxc_tmp_short_circuit_load_result_n65)
    {
      hxc_tmp_short_circuit_result_n27 = hxc_selectedSecond.hxc_value.hxc_a == 40;
    }
    bool hxc_tmp_short_circuit_load_result_n67 = hxc_tmp_short_circuit_result_n27;
    bool hxc_tmp_short_circuit_result_n28 = hxc_tmp_short_circuit_load_result_n67;
    if (hxc_tmp_short_circuit_load_result_n67)
    {
      hxc_tmp_short_circuit_result_n28 = hxc_selectedSecond.hxc_order == 2;
    }
    bool hxc_tmp_short_circuit_load_result_n69 = hxc_tmp_short_circuit_result_n28;
    bool hxc_tmp_short_circuit_result_n29 = hxc_tmp_short_circuit_load_result_n69;
    if (hxc_tmp_short_circuit_load_result_n69)
    {
      int32_t hxc_tmp_call_result_n72 = hxc_AggregateFixture_conditionalRecordSum(true, hxc_first, (struct hxc_OrderA){ .hxc_a = 40, .hxc_z = 30 });
      hxc_tmp_short_circuit_result_n29 = hxc_tmp_call_result_n72 == 7;
    }
    bool hxc_tmp_short_circuit_load_result_n73 = hxc_tmp_short_circuit_result_n29;
    bool hxc_tmp_short_circuit_result_n30 = hxc_tmp_short_circuit_load_result_n73;
    if (hxc_tmp_short_circuit_load_result_n73)
    {
      int32_t hxc_tmp_call_result_n76 = hxc_AggregateFixture_conditionalRecordSum(false, hxc_first, (struct hxc_OrderA){ .hxc_a = 40, .hxc_z = 30 });
      hxc_tmp_short_circuit_result_n30 = hxc_tmp_call_result_n76 == 70;
    }
    bool hxc_tmp_short_circuit_load_result_n77 = hxc_tmp_short_circuit_result_n30;
    bool hxc_tmp_short_circuit_result_n31 = hxc_tmp_short_circuit_load_result_n77;
    if (hxc_tmp_short_circuit_load_result_n77)
    {
      struct hxc_OrderA hxc_tmp_call_result_n80 = hxc_AggregateFixture_nestedRecord(true, false, hxc_first, (struct hxc_OrderA){ .hxc_a = 40, .hxc_z = 30 });
      int32_t hxc_tmp_call_result_n81 = hxc_AggregateFixture_sum(hxc_tmp_call_result_n80);
      hxc_tmp_short_circuit_result_n31 = hxc_tmp_call_result_n81 == 70;
    }
    bool hxc_tmp_short_circuit_load_result_n82 = hxc_tmp_short_circuit_result_n31;
    bool hxc_tmp_short_circuit_result_n32 = hxc_tmp_short_circuit_load_result_n82;
    if (hxc_tmp_short_circuit_load_result_n82)
    {
      struct hxc_OrderA hxc_tmp_call_result_n85 = hxc_AggregateFixture_nestedRecord(false, true, hxc_first, (struct hxc_OrderA){ .hxc_a = 40, .hxc_z = 30 });
      int32_t hxc_tmp_call_result_n86 = hxc_AggregateFixture_sum(hxc_tmp_call_result_n85);
      hxc_tmp_short_circuit_result_n32 = hxc_tmp_call_result_n86 == 42;
    }
    bool hxc_tmp_short_circuit_load_result_n87 = hxc_tmp_short_circuit_result_n32;
    bool hxc_tmp_short_circuit_result_n33 = hxc_tmp_short_circuit_load_result_n87;
    if (hxc_tmp_short_circuit_load_result_n87)
    {
      struct hxc_ActorPhase hxc_tmp_call_result_n88 = hxc_AggregateFixture_selectPhase(true, 9);
      struct hxc_ActorRecord hxc_tmp_call_result_n89 = hxc_AggregateFixture_makeActor(hxc_tmp_call_result_n88);
      int32_t hxc_tmp_call_result_n90 = hxc_AggregateFixture_actorSpeed(hxc_tmp_call_result_n89);
      hxc_tmp_short_circuit_result_n33 = hxc_tmp_call_result_n90 == 9;
    }
    bool hxc_tmp_short_circuit_load_result_n91 = hxc_tmp_short_circuit_result_n33;
    bool hxc_tmp_short_circuit_result_n34 = hxc_tmp_short_circuit_load_result_n91;
    if (hxc_tmp_short_circuit_load_result_n91)
    {
      struct hxc_ActorPhase hxc_tmp_call_result_n92 = hxc_AggregateFixture_selectPhase(false, 9);
      struct hxc_ActorRecord hxc_tmp_call_result_n93 = hxc_AggregateFixture_makeActor(hxc_tmp_call_result_n92);
      int32_t hxc_tmp_call_result_n94 = hxc_AggregateFixture_actorSpeed(hxc_tmp_call_result_n93);
      hxc_tmp_short_circuit_result_n34 = hxc_tmp_call_result_n94 == 0;
    }
    if (!!hxc_tmp_short_circuit_result_n34)
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

struct hxc_ActorRecord hxc_AggregateFixture_makeActor(struct hxc_ActorPhase hxc_phase)
{
  return (struct hxc_ActorRecord){ .hxc_phase = hxc_phase };
}

struct hxc_FlowRecord hxc_AggregateFixture_makeFlowRecord(int32_t hxc_first, int32_t hxc_second, int32_t hxc_third)
{
  int32_t hxc_order = 0;
  hxc_order = hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(hxc_order, 10), 1);
  int32_t hxc_tmp_conditional_result_n4 = 0;
  if (hxc_first < 0)
  {
    hxc_tmp_conditional_result_n4 = hxc_i32_negate_wrapping(hxc_first);
  }
  else
  {
    hxc_tmp_conditional_result_n4 = hxc_first;
  }
  int32_t hxc_tmp_record_field_first_n5 = hxc_tmp_conditional_result_n4;
  hxc_order = hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(hxc_order, 10), 2);
  int32_t hxc_tmp_conditional_result_n6 = 0;
  if (hxc_second < 0)
  {
    hxc_tmp_conditional_result_n6 = hxc_i32_negate_wrapping(hxc_second);
  }
  else
  {
    hxc_tmp_conditional_result_n6 = hxc_second;
  }
  int32_t hxc_tmp_record_field_second_n7 = hxc_tmp_conditional_result_n6;
  hxc_order = hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(hxc_order, 10), 3);
  int32_t hxc_tmp_conditional_result_n8 = 0;
  if (hxc_third < 0)
  {
    hxc_tmp_conditional_result_n8 = hxc_i32_negate_wrapping(hxc_third);
  }
  else
  {
    hxc_tmp_conditional_result_n8 = hxc_third;
  }
  int32_t hxc_tmp_conditional_load_result_n5 = hxc_tmp_conditional_result_n8;
  int32_t hxc_tmp_load_result_n6 = hxc_order;
  int32_t hxc_tmp_record_field_first_load_result_n7 = hxc_tmp_record_field_first_n5;
  return (struct hxc_FlowRecord){ .hxc_first = hxc_tmp_record_field_first_load_result_n7, .hxc_order = hxc_tmp_load_result_n6, .hxc_second = hxc_tmp_record_field_second_n7, .hxc_third = hxc_tmp_conditional_load_result_n5 };
}

struct hxc_SwitchRecord hxc_AggregateFixture_makeSwitch(int32_t hxc_state)
{
  return (struct hxc_SwitchRecord){ .hxc_state = hxc_state };
}

struct hxc_OrderA hxc_AggregateFixture_nestedRecord(bool hxc_chooseOuter, bool hxc_chooseInner, struct hxc_OrderA hxc_first, struct hxc_OrderA hxc_second)
{
  struct hxc_OrderA hxc_tmp_conditional_result_n4;
  if (hxc_chooseOuter)
  {
    struct hxc_OrderA hxc_tmp_conditional_result_n5;
    if (hxc_chooseInner)
    {
      hxc_tmp_conditional_result_n5 = hxc_first;
    }
    else
    {
      hxc_tmp_conditional_result_n5 = hxc_second;
    }
    hxc_tmp_conditional_result_n4 = hxc_tmp_conditional_result_n5;
  }
  else
  {
    hxc_tmp_conditional_result_n4 = (struct hxc_OrderA){ .hxc_a = 22, .hxc_z = 20 };
  }
  return hxc_tmp_conditional_result_n4;
}

struct hxc_optional_OrderA hxc_AggregateFixture_noPoint(void)
{
  return (struct hxc_optional_OrderA){ .hxc_has_value = false };
}

struct hxc_OptionalEnvelope hxc_AggregateFixture_optionalEnvelope(struct hxc_optional_OrderA hxc_value)
{
  return (struct hxc_OptionalEnvelope){ .hxc_point = hxc_value };
}

int32_t hxc_AggregateFixture_optionalSum(struct hxc_optional_OrderA hxc_value)
{
  int32_t hxc_tmp_conditional_result_n1 = 0;
  if (!hxc_value.hxc_has_value)
  {
    hxc_tmp_conditional_result_n1 = 0;
  }
  else
  {
    if (!hxc_value.hxc_has_value)
    {
      abort();
    }
    hxc_tmp_conditional_result_n1 = hxc_i32_add_wrapping(hxc_value.hxc_value.hxc_a, hxc_value.hxc_value.hxc_z);
  }
  return hxc_tmp_conditional_result_n1;
}

struct hxc_ActorPhase hxc_AggregateFixture_selectPhase(bool hxc_moving, int32_t hxc_speed)
{
  struct hxc_ActorPhase hxc_tmp_conditional_result_n2;
  if (hxc_moving)
  {
    hxc_tmp_conditional_result_n2 = (struct hxc_ActorPhase){ .hxc_tag = hxc_ActorPhase_Moving, .hxc_payload.hxc_Moving.hxc_speed = hxc_speed };
  }
  else
  {
    hxc_tmp_conditional_result_n2 = (struct hxc_ActorPhase){ .hxc_tag = hxc_ActorPhase_Waiting };
  }
  return hxc_tmp_conditional_result_n2;
}

struct hxc_ConditionalRecord hxc_AggregateFixture_selectRecord(bool hxc_chooseFirst, struct hxc_OrderA hxc_first, struct hxc_OrderA hxc_second)
{
  int32_t hxc_order = 0;
  struct hxc_OrderA hxc_tmp_conditional_result_n5;
  if (hxc_chooseFirst)
  {
    hxc_order = hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(hxc_order, 10), 1);
    hxc_tmp_conditional_result_n5 = hxc_first;
  }
  else
  {
    hxc_order = hxc_i32_add_wrapping(hxc_i32_multiply_wrapping(hxc_order, 10), 2);
    hxc_tmp_conditional_result_n5 = hxc_second;
  }
  struct hxc_OrderA hxc_selected = hxc_tmp_conditional_result_n5;
  struct hxc_OrderA hxc_tmp_load_result_n3 = hxc_selected;
  return (struct hxc_ConditionalRecord){ .hxc_order = hxc_order, .hxc_value = hxc_tmp_load_result_n3 };
}

struct hxc_optional_OrderA hxc_AggregateFixture_somePoint(struct hxc_OrderA hxc_value)
{
  return (struct hxc_optional_OrderA){ .hxc_has_value = true, .hxc_value = hxc_value };
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
