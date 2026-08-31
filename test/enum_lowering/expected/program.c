#include "hxc/program.h"

struct hxc_gc hxc_program_gc = HXC_GC_INITIALIZER;

struct hxc_gc_thread hxc_program_gc_thread = HXC_GC_THREAD_INITIALIZER;

_Static_assert(offsetof(struct hxc_RecursiveActionChoice, hxc_actions) == 0, "closed record hxc_RecursiveActionChoice first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_RecursiveActionChoice) >= _Alignof(struct hxc_array_ref *), "closed record hxc_RecursiveActionChoice alignment admits field 0");

_Static_assert(offsetof(struct hxc_RecursiveActionChoice, hxc_weight) >= offsetof(struct hxc_RecursiveActionChoice, hxc_actions) + sizeof(struct hxc_array_ref *), "closed record hxc_RecursiveActionChoice field 1 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_RecursiveActionChoice) >= _Alignof(int32_t), "closed record hxc_RecursiveActionChoice alignment admits field 1");

_Static_assert(sizeof(struct hxc_RecursiveActionChoice) >= offsetof(struct hxc_RecursiveActionChoice, hxc_weight) + sizeof(int32_t), "closed record hxc_RecursiveActionChoice size contains its final field");

_Static_assert(offsetof(struct hxc_StrictCarrierHolder, hxc_value) == 0, "closed record hxc_StrictCarrierHolder first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_StrictCarrierHolder) >= _Alignof(struct hxc_StrictCarrier), "closed record hxc_StrictCarrierHolder alignment admits field 0");

_Static_assert(sizeof(struct hxc_StrictCarrierHolder) >= offsetof(struct hxc_StrictCarrierHolder, hxc_value) + sizeof(struct hxc_StrictCarrier), "closed record hxc_StrictCarrierHolder size contains its final field");

_Static_assert(offsetof(struct hxc_EnumFixture_StackClosure, hxc_invoke) == 0, "closed record hxc_EnumFixture_StackClosure first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_EnumFixture_StackClosure) >= _Alignof(struct hxc_Option_h95f1c4a28dac (*)(void *, int32_t)), "closed record hxc_EnumFixture_StackClosure alignment admits field 0");

_Static_assert(offsetof(struct hxc_EnumFixture_StackClosure, hxc_context) >= offsetof(struct hxc_EnumFixture_StackClosure, hxc_invoke) + sizeof(struct hxc_Option_h95f1c4a28dac (*)(void *, int32_t)), "closed record hxc_EnumFixture_StackClosure field 1 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_EnumFixture_StackClosure) >= _Alignof(void *), "closed record hxc_EnumFixture_StackClosure alignment admits field 1");

_Static_assert(sizeof(struct hxc_EnumFixture_StackClosure) >= offsetof(struct hxc_EnumFixture_StackClosure, hxc_context) + sizeof(void *), "closed record hxc_EnumFixture_StackClosure size contains its final field");

_Static_assert(offsetof(struct hxc_Rule, hxc_actions) == 0, "closed record hxc_Rule first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_Rule) >= _Alignof(struct hxc_array_ref *), "closed record hxc_Rule alignment admits field 0");

_Static_assert(offsetof(struct hxc_Rule, hxc_chain) >= offsetof(struct hxc_Rule, hxc_actions) + sizeof(struct hxc_array_ref *), "closed record hxc_Rule field 1 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_Rule) >= _Alignof(struct hxc_Chain), "closed record hxc_Rule alignment admits field 1");

_Static_assert(offsetof(struct hxc_Rule, hxc_choices) >= offsetof(struct hxc_Rule, hxc_chain) + sizeof(struct hxc_Chain), "closed record hxc_Rule field 2 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_Rule) >= _Alignof(struct hxc_Choices), "closed record hxc_Rule alignment admits field 2");

_Static_assert(sizeof(struct hxc_Rule) >= offsetof(struct hxc_Rule, hxc_choices) + sizeof(struct hxc_Choices), "closed record hxc_Rule size contains its final field");

_Static_assert(offsetof(struct hxc_RecursiveActionPlan, hxc_actions) == 0, "closed record hxc_RecursiveActionPlan first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_RecursiveActionPlan) >= _Alignof(struct hxc_array_ref *), "closed record hxc_RecursiveActionPlan alignment admits field 0");

_Static_assert(sizeof(struct hxc_RecursiveActionPlan) >= offsetof(struct hxc_RecursiveActionPlan, hxc_actions) + sizeof(struct hxc_array_ref *), "closed record hxc_RecursiveActionPlan size contains its final field");

_Static_assert(hxc_Option_None_h00cd578bb80f == 0, "enum hxc_Option_ha0e4b5dcc139 case None retains its Haxe discriminant");

_Static_assert(hxc_Option_Some_h33493695ace2 == 1, "enum hxc_Option_ha0e4b5dcc139 case Some retains its Haxe discriminant");

_Static_assert(offsetof(struct hxc_Option_ha0e4b5dcc139, hxc_tag) == 0, "tagged enum hxc_Option_ha0e4b5dcc139 begins with its discriminant");

_Static_assert(offsetof(struct hxc_Option_ha0e4b5dcc139, hxc_payload) >= sizeof(enum hxc_Option_tag_h4f842caea9db), "tagged enum hxc_Option_ha0e4b5dcc139 payload follows its discriminant");

_Static_assert(sizeof(struct hxc_Option_ha0e4b5dcc139) >= offsetof(struct hxc_Option_ha0e4b5dcc139, hxc_payload) + sizeof(union hxc_Option_payload_ha68af457b79c), "tagged enum hxc_Option_ha0e4b5dcc139 contains its payload union");

_Static_assert(offsetof(union hxc_Option_payload_ha68af457b79c, hxc_Some) == 0, "tagged enum hxc_Option_ha0e4b5dcc139 case Some begins at union offset zero");

_Static_assert(offsetof(struct hxc_Option_Some_payload_hc2b8e9073c59, hxc_value) == 0, "tagged enum hxc_Option_ha0e4b5dcc139 case Some first payload begins at zero");

_Static_assert(_Alignof(struct hxc_Option_Some_payload_hc2b8e9073c59) >= _Alignof(bool), "tagged enum hxc_Option_ha0e4b5dcc139 case Some admits payload 0 alignment");

_Static_assert(hxc_Option_None_hdcfb48028a4b == 0, "enum hxc_Option_h2a07afaff02e case None retains its Haxe discriminant");

_Static_assert(hxc_Option_Some_ha8dd5a59e40a == 1, "enum hxc_Option_h2a07afaff02e case Some retains its Haxe discriminant");

_Static_assert(offsetof(struct hxc_Option_h2a07afaff02e, hxc_tag) == 0, "tagged enum hxc_Option_h2a07afaff02e begins with its discriminant");

_Static_assert(offsetof(struct hxc_Option_h2a07afaff02e, hxc_payload) >= sizeof(enum hxc_Option_tag_hff067ac061db), "tagged enum hxc_Option_h2a07afaff02e payload follows its discriminant");

_Static_assert(sizeof(struct hxc_Option_h2a07afaff02e) >= offsetof(struct hxc_Option_h2a07afaff02e, hxc_payload) + sizeof(union hxc_Option_payload_hbc7d11cfb27e), "tagged enum hxc_Option_h2a07afaff02e contains its payload union");

_Static_assert(offsetof(union hxc_Option_payload_hbc7d11cfb27e, hxc_Some) == 0, "tagged enum hxc_Option_h2a07afaff02e case Some begins at union offset zero");

_Static_assert(offsetof(struct hxc_Option_Some_payload_h663331e4468e, hxc_value) == 0, "tagged enum hxc_Option_h2a07afaff02e case Some first payload begins at zero");

_Static_assert(_Alignof(struct hxc_Option_Some_payload_h663331e4468e) >= _Alignof(struct hxc_Rule), "tagged enum hxc_Option_h2a07afaff02e case Some admits payload 0 alignment");

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

_Static_assert(hxc_RecursiveAction_LeafAction == 0, "enum hxc_RecursiveAction case LeafAction retains its Haxe discriminant");

_Static_assert(hxc_RecursiveAction_ChooseAction == 1, "enum hxc_RecursiveAction case ChooseAction retains its Haxe discriminant");

_Static_assert(offsetof(struct hxc_RecursiveAction, hxc_tag) == 0, "tagged enum hxc_RecursiveAction begins with its discriminant");

_Static_assert(offsetof(struct hxc_RecursiveAction, hxc_payload) >= sizeof(enum hxc_RecursiveAction_tag), "tagged enum hxc_RecursiveAction payload follows its discriminant");

_Static_assert(sizeof(struct hxc_RecursiveAction) >= offsetof(struct hxc_RecursiveAction, hxc_payload) + sizeof(union hxc_RecursiveAction_payload), "tagged enum hxc_RecursiveAction contains its payload union");

_Static_assert(offsetof(union hxc_RecursiveAction_payload, hxc_LeafAction) == 0, "tagged enum hxc_RecursiveAction case LeafAction begins at union offset zero");

_Static_assert(offsetof(struct hxc_RecursiveAction_LeafAction_payload, hxc_value) == 0, "tagged enum hxc_RecursiveAction case LeafAction first payload begins at zero");

_Static_assert(_Alignof(struct hxc_RecursiveAction_LeafAction_payload) >= _Alignof(int32_t), "tagged enum hxc_RecursiveAction case LeafAction admits payload 0 alignment");

_Static_assert(offsetof(union hxc_RecursiveAction_payload, hxc_ChooseAction) == 0, "tagged enum hxc_RecursiveAction case ChooseAction begins at union offset zero");

_Static_assert(offsetof(struct hxc_RecursiveAction_ChooseAction_payload, hxc_choices) == 0, "tagged enum hxc_RecursiveAction case ChooseAction first payload begins at zero");

_Static_assert(_Alignof(struct hxc_RecursiveAction_ChooseAction_payload) >= _Alignof(struct hxc_array_ref *), "tagged enum hxc_RecursiveAction case ChooseAction admits payload 0 alignment");

_Static_assert(hxc_IdentityKind_FirstIdentity == 0, "enum hxc_IdentityKind case FirstIdentity retains its Haxe discriminant");

_Static_assert(hxc_IdentityKind_SecondIdentity == 1, "enum hxc_IdentityKind case SecondIdentity retains its Haxe discriminant");

_Static_assert(hxc_StrictCarrier_StrictEmpty == 0, "enum hxc_StrictCarrier case StrictEmpty retains its Haxe discriminant");

_Static_assert(hxc_StrictCarrier_StrictSmall == 1, "enum hxc_StrictCarrier case StrictSmall retains its Haxe discriminant");

_Static_assert(hxc_StrictCarrier_StrictWide == 2, "enum hxc_StrictCarrier case StrictWide retains its Haxe discriminant");

_Static_assert(offsetof(struct hxc_StrictCarrier, hxc_tag) == 0, "tagged enum hxc_StrictCarrier begins with its discriminant");

_Static_assert(offsetof(struct hxc_StrictCarrier, hxc_payload) >= sizeof(enum hxc_StrictCarrier_tag), "tagged enum hxc_StrictCarrier payload follows its discriminant");

_Static_assert(sizeof(struct hxc_StrictCarrier) >= offsetof(struct hxc_StrictCarrier, hxc_payload) + sizeof(union hxc_StrictCarrier_payload), "tagged enum hxc_StrictCarrier contains its payload union");

_Static_assert(offsetof(union hxc_StrictCarrier_payload, hxc_StrictSmall) == 0, "tagged enum hxc_StrictCarrier case StrictSmall begins at union offset zero");

_Static_assert(offsetof(struct hxc_StrictCarrier_StrictSmall_payload, hxc_value) == 0, "tagged enum hxc_StrictCarrier case StrictSmall first payload begins at zero");

_Static_assert(_Alignof(struct hxc_StrictCarrier_StrictSmall_payload) >= _Alignof(int32_t), "tagged enum hxc_StrictCarrier case StrictSmall admits payload 0 alignment");

_Static_assert(offsetof(union hxc_StrictCarrier_payload, hxc_StrictWide) == 0, "tagged enum hxc_StrictCarrier case StrictWide begins at union offset zero");

_Static_assert(offsetof(struct hxc_StrictCarrier_StrictWide_payload, hxc_first) == 0, "tagged enum hxc_StrictCarrier case StrictWide first payload begins at zero");

_Static_assert(_Alignof(struct hxc_StrictCarrier_StrictWide_payload) >= _Alignof(int32_t), "tagged enum hxc_StrictCarrier case StrictWide admits payload 0 alignment");

_Static_assert(offsetof(struct hxc_StrictCarrier_StrictWide_payload, hxc_second) >= offsetof(struct hxc_StrictCarrier_StrictWide_payload, hxc_first) + sizeof(int32_t), "tagged enum hxc_StrictCarrier case StrictWide payload 1 follows its predecessor");

_Static_assert(_Alignof(struct hxc_StrictCarrier_StrictWide_payload) >= _Alignof(int32_t), "tagged enum hxc_StrictCarrier case StrictWide admits payload 1 alignment");

_Static_assert(offsetof(struct hxc_StrictCarrier_StrictWide_payload, hxc_third) >= offsetof(struct hxc_StrictCarrier_StrictWide_payload, hxc_second) + sizeof(int32_t), "tagged enum hxc_StrictCarrier case StrictWide payload 2 follows its predecessor");

_Static_assert(_Alignof(struct hxc_StrictCarrier_StrictWide_payload) >= _Alignof(int32_t), "tagged enum hxc_StrictCarrier case StrictWide admits payload 2 alignment");

_Static_assert(offsetof(struct hxc_StrictCarrier_StrictWide_payload, hxc_fourth) >= offsetof(struct hxc_StrictCarrier_StrictWide_payload, hxc_third) + sizeof(int32_t), "tagged enum hxc_StrictCarrier case StrictWide payload 3 follows its predecessor");

_Static_assert(_Alignof(struct hxc_StrictCarrier_StrictWide_payload) >= _Alignof(int32_t), "tagged enum hxc_StrictCarrier case StrictWide admits payload 3 alignment");

_Static_assert(hxc_Choices_NoChoices == 0, "enum hxc_Choices case NoChoices retains its Haxe discriminant");

_Static_assert(hxc_Choices_ChoiceValues == 1, "enum hxc_Choices case ChoiceValues retains its Haxe discriminant");

_Static_assert(offsetof(struct hxc_Choices, hxc_tag) == 0, "tagged enum hxc_Choices begins with its discriminant");

_Static_assert(offsetof(struct hxc_Choices, hxc_payload) >= sizeof(enum hxc_Choices_tag), "tagged enum hxc_Choices payload follows its discriminant");

_Static_assert(sizeof(struct hxc_Choices) >= offsetof(struct hxc_Choices, hxc_payload) + sizeof(union hxc_Choices_payload), "tagged enum hxc_Choices contains its payload union");

_Static_assert(offsetof(union hxc_Choices_payload, hxc_ChoiceValues) == 0, "tagged enum hxc_Choices case ChoiceValues begins at union offset zero");

_Static_assert(offsetof(struct hxc_Choices_ChoiceValues_payload, hxc_values) == 0, "tagged enum hxc_Choices case ChoiceValues first payload begins at zero");

_Static_assert(_Alignof(struct hxc_Choices_ChoiceValues_payload) >= _Alignof(struct hxc_array_ref *), "tagged enum hxc_Choices case ChoiceValues admits payload 0 alignment");

_Static_assert(hxc_IdentityValue_FirstValue == 0, "enum hxc_IdentityValue case FirstValue retains its Haxe discriminant");

_Static_assert(hxc_IdentityValue_SecondValue == 1, "enum hxc_IdentityValue case SecondValue retains its Haxe discriminant");

_Static_assert(offsetof(struct hxc_IdentityValue, hxc_tag) == 0, "tagged enum hxc_IdentityValue begins with its discriminant");

_Static_assert(offsetof(struct hxc_IdentityValue, hxc_payload) >= sizeof(enum hxc_IdentityValue_tag), "tagged enum hxc_IdentityValue payload follows its discriminant");

_Static_assert(sizeof(struct hxc_IdentityValue) >= offsetof(struct hxc_IdentityValue, hxc_payload) + sizeof(union hxc_IdentityValue_payload), "tagged enum hxc_IdentityValue contains its payload union");

_Static_assert(offsetof(union hxc_IdentityValue_payload, hxc_FirstValue) == 0, "tagged enum hxc_IdentityValue case FirstValue begins at union offset zero");

_Static_assert(offsetof(struct hxc_IdentityValue_FirstValue_payload, hxc_value) == 0, "tagged enum hxc_IdentityValue case FirstValue first payload begins at zero");

_Static_assert(_Alignof(struct hxc_IdentityValue_FirstValue_payload) >= _Alignof(int32_t), "tagged enum hxc_IdentityValue case FirstValue admits payload 0 alignment");

_Static_assert(offsetof(union hxc_IdentityValue_payload, hxc_SecondValue) == 0, "tagged enum hxc_IdentityValue case SecondValue begins at union offset zero");

_Static_assert(offsetof(struct hxc_IdentityValue_SecondValue_payload, hxc_value) == 0, "tagged enum hxc_IdentityValue case SecondValue first payload begins at zero");

_Static_assert(_Alignof(struct hxc_IdentityValue_SecondValue_payload) >= _Alignof(int32_t), "tagged enum hxc_IdentityValue case SecondValue admits payload 0 alignment");

_Static_assert(hxc_RuleEnvelope_MissingRule == 0, "enum hxc_RuleEnvelope case MissingRule retains its Haxe discriminant");

_Static_assert(hxc_RuleEnvelope_WrappedRule == 1, "enum hxc_RuleEnvelope case WrappedRule retains its Haxe discriminant");

_Static_assert(offsetof(struct hxc_RuleEnvelope, hxc_tag) == 0, "tagged enum hxc_RuleEnvelope begins with its discriminant");

_Static_assert(offsetof(struct hxc_RuleEnvelope, hxc_payload) >= sizeof(enum hxc_RuleEnvelope_tag), "tagged enum hxc_RuleEnvelope payload follows its discriminant");

_Static_assert(sizeof(struct hxc_RuleEnvelope) >= offsetof(struct hxc_RuleEnvelope, hxc_payload) + sizeof(union hxc_RuleEnvelope_payload), "tagged enum hxc_RuleEnvelope contains its payload union");

_Static_assert(offsetof(union hxc_RuleEnvelope_payload, hxc_WrappedRule) == 0, "tagged enum hxc_RuleEnvelope case WrappedRule begins at union offset zero");

_Static_assert(offsetof(struct hxc_RuleEnvelope_WrappedRule_payload, hxc_rule) == 0, "tagged enum hxc_RuleEnvelope case WrappedRule first payload begins at zero");

_Static_assert(_Alignof(struct hxc_RuleEnvelope_WrappedRule_payload) >= _Alignof(struct hxc_Rule), "tagged enum hxc_RuleEnvelope case WrappedRule admits payload 0 alignment");

static void hxc_array_e4791f3e_trace(const void *hxc_array_e4791f3e_trace_object, hxc_trace_visit_fn hxc_array_e4791f3e_trace_visit, void *hxc_array_e4791f3e_trace_context)
{
  const struct hxc_array_ref *hxc_array_e4791f3e_trace_typed = (const struct hxc_array_ref *)hxc_array_e4791f3e_trace_object;
  size_t hxc_array_e4791f3e_trace_index = 0;
  while (hxc_array_e4791f3e_trace_index < hxc_array_e4791f3e_trace_typed->value.length)
  {
    if (((const struct hxc_RecursiveActionChoice *)hxc_array_e4791f3e_trace_typed->value.storage.memory)[hxc_array_e4791f3e_trace_index].hxc_actions != NULL)
    {
      hxc_array_e4791f3e_trace_visit(hxc_array_e4791f3e_trace_context, ((const struct hxc_RecursiveActionChoice *)hxc_array_e4791f3e_trace_typed->value.storage.memory)[hxc_array_e4791f3e_trace_index].hxc_actions);
    }
    hxc_array_e4791f3e_trace_index++;
  }
}

static void hxc_array_e4791f3e_finalize(void *hxc_array_e4791f3e_finalize_object)
{
  (void)hxc_array_ref_dispose_in_place((struct hxc_array_ref *)hxc_array_e4791f3e_finalize_object);
}

static void hxc_array_eaf5e746_trace(const void *hxc_array_eaf5e746_trace_object, hxc_trace_visit_fn hxc_array_eaf5e746_trace_visit, void *hxc_array_eaf5e746_trace_context)
{
  const struct hxc_array_ref *hxc_array_eaf5e746_trace_typed = (const struct hxc_array_ref *)hxc_array_eaf5e746_trace_object;
  size_t hxc_array_eaf5e746_trace_index = 0;
  while (hxc_array_eaf5e746_trace_index < hxc_array_eaf5e746_trace_typed->value.length)
  {
    switch (((const struct hxc_RecursiveAction *)hxc_array_eaf5e746_trace_typed->value.storage.memory)[hxc_array_eaf5e746_trace_index].hxc_tag) {
      case hxc_RecursiveAction_LeafAction:
        {
          break;
        }
      case hxc_RecursiveAction_ChooseAction:
        {
          if (((const struct hxc_RecursiveAction *)hxc_array_eaf5e746_trace_typed->value.storage.memory)[hxc_array_eaf5e746_trace_index].hxc_payload.hxc_ChooseAction.hxc_choices != NULL)
          {
            hxc_array_eaf5e746_trace_visit(hxc_array_eaf5e746_trace_context, ((const struct hxc_RecursiveAction *)hxc_array_eaf5e746_trace_typed->value.storage.memory)[hxc_array_eaf5e746_trace_index].hxc_payload.hxc_ChooseAction.hxc_choices);
          }
          break;
        }
    }
    hxc_array_eaf5e746_trace_index++;
  }
}

static void hxc_array_eaf5e746_finalize(void *hxc_array_eaf5e746_finalize_object)
{
  (void)hxc_array_ref_dispose_in_place((struct hxc_array_ref *)hxc_array_eaf5e746_finalize_object);
}

_Static_assert(sizeof(struct hxc_array_ref) % _Alignof(struct hxc_array_ref) == 0, "descriptor `array.e4791f3ed60810b12b61c8cbc1ceb97c55da777a819f42e55ea69e70c231ec6a` payload size must be a multiple of alignment");

const struct hxc_type_descriptor hxc_array_e4791f3e_descriptor = { .abi_version = HXC_TYPE_DESCRIPTOR_ABI_VERSION, .flags = HXC_TYPE_DESCRIPTOR_HAS_TRACE | HXC_TYPE_DESCRIPTOR_HAS_FINALIZER, .object_size = sizeof(struct hxc_array_ref), .object_alignment = _Alignof(struct hxc_array_ref), .trace = hxc_array_e4791f3e_trace, .finalize = hxc_array_e4791f3e_finalize };

_Static_assert(sizeof(struct hxc_array_ref) % _Alignof(struct hxc_array_ref) == 0, "descriptor `array.eaf5e746484d8e083db4dbe7f227834602df94e65db1ec592bd772f7b757bc3a` payload size must be a multiple of alignment");

const struct hxc_type_descriptor hxc_array_eaf5e746_descriptor = { .abi_version = HXC_TYPE_DESCRIPTOR_ABI_VERSION, .flags = HXC_TYPE_DESCRIPTOR_HAS_TRACE | HXC_TYPE_DESCRIPTOR_HAS_FINALIZER, .object_size = sizeof(struct hxc_array_ref), .object_alignment = _Alignof(struct hxc_array_ref), .trace = hxc_array_eaf5e746_trace, .finalize = hxc_array_eaf5e746_finalize };

hxc_status hxc_record_9f230b68_retain(void *hxc_l_value)
{
  hxc_status hxc_l_operation_status;
  hxc_l_operation_status = hxc_array_ref_retain((*(struct hxc_Rule *)hxc_l_value).hxc_actions);
  if (hxc_l_operation_status != HXC_STATUS_OK)
  {
    return hxc_l_operation_status;
  }
  hxc_l_operation_status = hxc_enum_39285fe9_retain(&(*(struct hxc_Rule *)hxc_l_value).hxc_chain);
  if (hxc_l_operation_status != HXC_STATUS_OK)
  {
    (void)hxc_array_ref_release((*(struct hxc_Rule *)hxc_l_value).hxc_actions);
    return hxc_l_operation_status;
  }
  hxc_l_operation_status = hxc_enum_d215f611_retain(&(*(struct hxc_Rule *)hxc_l_value).hxc_choices);
  if (hxc_l_operation_status != HXC_STATUS_OK)
  {
    (void)hxc_enum_39285fe9_destroy(&(*(struct hxc_Rule *)hxc_l_value).hxc_chain);
    (void)hxc_array_ref_release((*(struct hxc_Rule *)hxc_l_value).hxc_actions);
    return hxc_l_operation_status;
  }
  return HXC_STATUS_OK;
}

void hxc_record_9f230b68_destroy(void *hxc_l_value)
{
  (void)hxc_enum_d215f611_destroy(&(*(struct hxc_Rule *)hxc_l_value).hxc_choices);
  (void)hxc_enum_39285fe9_destroy(&(*(struct hxc_Rule *)hxc_l_value).hxc_chain);
  (void)hxc_array_ref_release((*(struct hxc_Rule *)hxc_l_value).hxc_actions);
}

hxc_status hxc_enum_24936704_retain(void *hxc_l_value)
{
  hxc_status hxc_l_operation_status;
  switch ((*(struct hxc_Option_h2a07afaff02e *)hxc_l_value).hxc_tag) {
    case hxc_Option_None_hdcfb48028a4b:
      {
        break;
      }
    case hxc_Option_Some_ha8dd5a59e40a:
      {
        hxc_l_operation_status = hxc_record_9f230b68_retain(&(*(struct hxc_Option_h2a07afaff02e *)hxc_l_value).hxc_payload.hxc_Some.hxc_value);
        if (hxc_l_operation_status != HXC_STATUS_OK)
        {
          return hxc_l_operation_status;
        }
        break;
      }
  }
  return HXC_STATUS_OK;
}

void hxc_enum_24936704_destroy(void *hxc_l_value)
{
  switch ((*(struct hxc_Option_h2a07afaff02e *)hxc_l_value).hxc_tag) {
    case hxc_Option_None_hdcfb48028a4b:
      {
        break;
      }
    case hxc_Option_Some_ha8dd5a59e40a:
      {
        (void)hxc_record_9f230b68_destroy(&(*(struct hxc_Option_h2a07afaff02e *)hxc_l_value).hxc_payload.hxc_Some.hxc_value);
        break;
      }
  }
}

hxc_status hxc_enum_39285fe9_retain(void *hxc_l_value)
{
  hxc_status hxc_l_operation_status;
  switch ((*(struct hxc_Chain *)hxc_l_value).hxc_tag) {
    case hxc_Chain_End:
      {
        break;
      }
    case hxc_Chain_Link:
      {
        hxc_l_operation_status = hxc_enum_39285fe9_retain_recursive_clone(&(*(struct hxc_Chain *)hxc_l_value).hxc_payload.hxc_Link.hxc_next);
        if (hxc_l_operation_status != HXC_STATUS_OK)
        {
          return hxc_l_operation_status;
        }
        break;
      }
  }
  return HXC_STATUS_OK;
}

void hxc_enum_39285fe9_destroy(void *hxc_l_value)
{
  switch ((*(struct hxc_Chain *)hxc_l_value).hxc_tag) {
    case hxc_Chain_End:
      {
        break;
      }
    case hxc_Chain_Link:
      {
        (void)hxc_enum_39285fe9_destroy_recursive_destroy(&(*(struct hxc_Chain *)hxc_l_value).hxc_payload.hxc_Link.hxc_next);
        break;
      }
  }
}

hxc_status hxc_enum_39285fe9_retain_recursive_clone(void *hxc_enum_39285fe9_retain_recursive_clone_slot)
{
  struct hxc_Chain **hxc_enum_39285fe9_retain_recursive_clone_typed_slot = (struct hxc_Chain **)hxc_enum_39285fe9_retain_recursive_clone_slot;
  struct hxc_Chain *hxc_enum_39285fe9_retain_recursive_clone_source = *hxc_enum_39285fe9_retain_recursive_clone_typed_slot;
  struct hxc_Chain *hxc_enum_39285fe9_retain_recursive_clone_copy = NULL;
  hxc_allocator hxc_enum_39285fe9_retain_recursive_clone_allocator = hxc_default_allocator();
  hxc_status hxc_enum_39285fe9_retain_recursive_clone_operation_status;
  hxc_enum_39285fe9_retain_recursive_clone_operation_status = hxc_alloc(&hxc_enum_39285fe9_retain_recursive_clone_allocator, sizeof(struct hxc_Chain), _Alignof(struct hxc_Chain), (void **)&hxc_enum_39285fe9_retain_recursive_clone_copy);
  if (hxc_enum_39285fe9_retain_recursive_clone_operation_status != HXC_STATUS_OK)
  {
    return hxc_enum_39285fe9_retain_recursive_clone_operation_status;
  }
  *hxc_enum_39285fe9_retain_recursive_clone_copy = *hxc_enum_39285fe9_retain_recursive_clone_source;
  hxc_enum_39285fe9_retain_recursive_clone_operation_status = hxc_enum_39285fe9_retain(hxc_enum_39285fe9_retain_recursive_clone_copy);
  if (hxc_enum_39285fe9_retain_recursive_clone_operation_status != HXC_STATUS_OK)
  {
    (void)hxc_free(&hxc_enum_39285fe9_retain_recursive_clone_allocator, hxc_enum_39285fe9_retain_recursive_clone_copy, sizeof(struct hxc_Chain), _Alignof(struct hxc_Chain));
    return hxc_enum_39285fe9_retain_recursive_clone_operation_status;
  }
  *hxc_enum_39285fe9_retain_recursive_clone_typed_slot = hxc_enum_39285fe9_retain_recursive_clone_copy;
  return HXC_STATUS_OK;
}

void hxc_enum_39285fe9_destroy_recursive_destroy(void *hxc_enum_39285fe9_destroy_recursive_destroy_slot)
{
  struct hxc_Chain **hxc_enum_39285fe9_destroy_recursive_destroy_typed_slot = (struct hxc_Chain **)hxc_enum_39285fe9_destroy_recursive_destroy_slot;
  struct hxc_Chain *hxc_enum_39285fe9_destroy_recursive_destroy_owned = *hxc_enum_39285fe9_destroy_recursive_destroy_typed_slot;
  hxc_allocator hxc_enum_39285fe9_destroy_recursive_destroy_allocator = hxc_default_allocator();
  hxc_enum_39285fe9_destroy(hxc_enum_39285fe9_destroy_recursive_destroy_owned);
  (void)hxc_free(&hxc_enum_39285fe9_destroy_recursive_destroy_allocator, hxc_enum_39285fe9_destroy_recursive_destroy_owned, sizeof(struct hxc_Chain), _Alignof(struct hxc_Chain));
  *hxc_enum_39285fe9_destroy_recursive_destroy_typed_slot = NULL;
}

hxc_status hxc_enum_d215f611_retain(void *hxc_l_value)
{
  hxc_status hxc_l_operation_status;
  switch ((*(struct hxc_Choices *)hxc_l_value).hxc_tag) {
    case hxc_Choices_NoChoices:
      {
        break;
      }
    case hxc_Choices_ChoiceValues:
      {
        hxc_l_operation_status = hxc_array_ref_retain((*(struct hxc_Choices *)hxc_l_value).hxc_payload.hxc_ChoiceValues.hxc_values);
        if (hxc_l_operation_status != HXC_STATUS_OK)
        {
          return hxc_l_operation_status;
        }
        break;
      }
  }
  return HXC_STATUS_OK;
}

void hxc_enum_d215f611_destroy(void *hxc_l_value)
{
  switch ((*(struct hxc_Choices *)hxc_l_value).hxc_tag) {
    case hxc_Choices_NoChoices:
      {
        break;
      }
    case hxc_Choices_ChoiceValues:
      {
        (void)hxc_array_ref_release((*(struct hxc_Choices *)hxc_l_value).hxc_payload.hxc_ChoiceValues.hxc_values);
        break;
      }
  }
}

hxc_status hxc_enum_ffce8027_retain(void *hxc_l_value)
{
  hxc_status hxc_l_operation_status;
  switch ((*(struct hxc_RuleEnvelope *)hxc_l_value).hxc_tag) {
    case hxc_RuleEnvelope_MissingRule:
      {
        break;
      }
    case hxc_RuleEnvelope_WrappedRule:
      {
        hxc_l_operation_status = hxc_record_9f230b68_retain(&(*(struct hxc_RuleEnvelope *)hxc_l_value).hxc_payload.hxc_WrappedRule.hxc_rule);
        if (hxc_l_operation_status != HXC_STATUS_OK)
        {
          return hxc_l_operation_status;
        }
        break;
      }
  }
  return HXC_STATUS_OK;
}

void hxc_enum_ffce8027_destroy(void *hxc_l_value)
{
  switch ((*(struct hxc_RuleEnvelope *)hxc_l_value).hxc_tag) {
    case hxc_RuleEnvelope_MissingRule:
      {
        break;
      }
    case hxc_RuleEnvelope_WrappedRule:
      {
        (void)hxc_record_9f230b68_destroy(&(*(struct hxc_RuleEnvelope *)hxc_l_value).hxc_payload.hxc_WrappedRule.hxc_rule);
        break;
      }
  }
}

hxc_status hxc_array_400559e4_element_copy(void *hxc_l_context, void *hxc_l_destination, const void *hxc_l_source)
{
  (void)hxc_l_context;
  hxc_status hxc_l_operation_status;
  *(struct hxc_Rule *)hxc_l_destination = *(const struct hxc_Rule *)hxc_l_source;
  hxc_l_operation_status = hxc_array_ref_retain((*(struct hxc_Rule *)hxc_l_destination).hxc_actions);
  if (hxc_l_operation_status != HXC_STATUS_OK)
  {
    return hxc_l_operation_status;
  }
  hxc_l_operation_status = hxc_enum_39285fe9_retain(&(*(struct hxc_Rule *)hxc_l_destination).hxc_chain);
  if (hxc_l_operation_status != HXC_STATUS_OK)
  {
    (void)hxc_array_ref_release((*(struct hxc_Rule *)hxc_l_destination).hxc_actions);
    return hxc_l_operation_status;
  }
  hxc_l_operation_status = hxc_enum_d215f611_retain(&(*(struct hxc_Rule *)hxc_l_destination).hxc_choices);
  if (hxc_l_operation_status != HXC_STATUS_OK)
  {
    (void)hxc_enum_39285fe9_destroy(&(*(struct hxc_Rule *)hxc_l_destination).hxc_chain);
    (void)hxc_array_ref_release((*(struct hxc_Rule *)hxc_l_destination).hxc_actions);
    return hxc_l_operation_status;
  }
  return HXC_STATUS_OK;
}

hxc_status hxc_array_400559e4_element_assign(void *hxc_l_context, void *hxc_l_destination, const void *hxc_l_source)
{
  (void)hxc_l_context;
  if (hxc_l_destination == hxc_l_source)
  {
    return HXC_STATUS_OK;
  }
  hxc_status hxc_l_operation_status;
  struct hxc_Rule hxc_array_400559e4_element_assign_replacement = *(const struct hxc_Rule *)hxc_l_source;
  hxc_l_operation_status = hxc_array_ref_retain(hxc_array_400559e4_element_assign_replacement.hxc_actions);
  if (hxc_l_operation_status != HXC_STATUS_OK)
  {
    return hxc_l_operation_status;
  }
  hxc_l_operation_status = hxc_enum_39285fe9_retain(&hxc_array_400559e4_element_assign_replacement.hxc_chain);
  if (hxc_l_operation_status != HXC_STATUS_OK)
  {
    (void)hxc_array_ref_release(hxc_array_400559e4_element_assign_replacement.hxc_actions);
    return hxc_l_operation_status;
  }
  hxc_l_operation_status = hxc_enum_d215f611_retain(&hxc_array_400559e4_element_assign_replacement.hxc_choices);
  if (hxc_l_operation_status != HXC_STATUS_OK)
  {
    (void)hxc_enum_39285fe9_destroy(&hxc_array_400559e4_element_assign_replacement.hxc_chain);
    (void)hxc_array_ref_release(hxc_array_400559e4_element_assign_replacement.hxc_actions);
    return hxc_l_operation_status;
  }
  (void)hxc_enum_d215f611_destroy(&(*(struct hxc_Rule *)hxc_l_destination).hxc_choices);
  (void)hxc_enum_39285fe9_destroy(&(*(struct hxc_Rule *)hxc_l_destination).hxc_chain);
  (void)hxc_array_ref_release((*(struct hxc_Rule *)hxc_l_destination).hxc_actions);
  *(struct hxc_Rule *)hxc_l_destination = hxc_array_400559e4_element_assign_replacement;
  return HXC_STATUS_OK;
}

void hxc_array_400559e4_element_destroy(void *hxc_l_context, void *hxc_l_element)
{
  (void)hxc_l_context;
  (void)hxc_enum_d215f611_destroy(&(*(struct hxc_Rule *)hxc_l_element).hxc_choices);
  (void)hxc_enum_39285fe9_destroy(&(*(struct hxc_Rule *)hxc_l_element).hxc_chain);
  (void)hxc_array_ref_release((*(struct hxc_Rule *)hxc_l_element).hxc_actions);
}

hxc_status hxc_array_84c38722_element_copy(void *hxc_l_context, void *hxc_l_destination, const void *hxc_l_source)
{
  (void)hxc_l_context;
  hxc_status hxc_l_operation_status;
  *(struct hxc_RuleEnvelope *)hxc_l_destination = *(const struct hxc_RuleEnvelope *)hxc_l_source;
  hxc_l_operation_status = hxc_enum_ffce8027_retain(&*(struct hxc_RuleEnvelope *)hxc_l_destination);
  if (hxc_l_operation_status != HXC_STATUS_OK)
  {
    return hxc_l_operation_status;
  }
  return HXC_STATUS_OK;
}

hxc_status hxc_array_84c38722_element_assign(void *hxc_l_context, void *hxc_l_destination, const void *hxc_l_source)
{
  (void)hxc_l_context;
  if (hxc_l_destination == hxc_l_source)
  {
    return HXC_STATUS_OK;
  }
  hxc_status hxc_l_operation_status;
  struct hxc_RuleEnvelope hxc_array_84c38722_element_assign_replacement = *(const struct hxc_RuleEnvelope *)hxc_l_source;
  hxc_l_operation_status = hxc_enum_ffce8027_retain(&hxc_array_84c38722_element_assign_replacement);
  if (hxc_l_operation_status != HXC_STATUS_OK)
  {
    return hxc_l_operation_status;
  }
  hxc_enum_ffce8027_destroy(&*(struct hxc_RuleEnvelope *)hxc_l_destination);
  *(struct hxc_RuleEnvelope *)hxc_l_destination = hxc_array_84c38722_element_assign_replacement;
  return HXC_STATUS_OK;
}

void hxc_array_84c38722_element_destroy(void *hxc_l_context, void *hxc_l_element)
{
  (void)hxc_l_context;
  hxc_enum_ffce8027_destroy(&*(struct hxc_RuleEnvelope *)hxc_l_element);
}

struct hxc_Option_h95f1c4a28dac hxc_EnumFixture_applyOption(int32_t hxc_l_value, struct hxc_EnumFixture_StackClosure hxc_l_constructor)
{
  struct hxc_Option_h95f1c4a28dac hxc_l_tmp_indirect_call_result_n0 = hxc_l_constructor.hxc_invoke(hxc_l_constructor.hxc_context, hxc_l_value);
  return hxc_l_tmp_indirect_call_result_n0;
}

int32_t hxc_EnumFixture_boolOptionValue(struct hxc_Option_ha0e4b5dcc139 hxc_l_value_hb6e6538779c8)
{
  int32_t hxc_l_tmp_enum_switch_result_n1 = 0;
  switch (hxc_l_value_hb6e6538779c8.hxc_tag) {
    case hxc_Option_None_h00cd578bb80f:
      {
        hxc_l_tmp_enum_switch_result_n1 = -1;
        break;
      }
    case hxc_Option_Some_h33493695ace2:
      {
        if (hxc_l_value_hb6e6538779c8.hxc_tag != hxc_Option_Some_h33493695ace2)
        {
          abort();
        }
        bool hxc_l_tmp_enum_payload_project_n0 = hxc_l_value_hb6e6538779c8.hxc_payload.hxc_Some.hxc_value;
        bool hxc_l_value_h85a49fca2ea0 = hxc_l_tmp_enum_payload_project_n0;
        bool hxc_l_payload = hxc_l_value_h85a49fca2ea0;
        bool hxc_l_tmp_load_result_n2 = hxc_l_payload;
        int32_t hxc_l_tmp_conditional_result_n4 = 0;
        if (hxc_l_tmp_load_result_n2)
        {
          hxc_l_tmp_conditional_result_n4 = 1;
        }
        else
        {
          hxc_l_tmp_conditional_result_n4 = 0;
        }
        hxc_l_tmp_enum_switch_result_n1 = hxc_l_tmp_conditional_result_n4;
        break;
      }
    default:
      {
        abort();
      }
  }
  return hxc_l_tmp_enum_switch_result_n1;
}

int32_t hxc_EnumFixture_chainValue(struct hxc_Chain hxc_l_value_h10794fed7059)
{
  struct hxc_Chain hxc_l_next_hcce74b90370b = { 0 };
  struct hxc_Chain hxc_l_next_h24eca731d2de = { 0 };
  int32_t hxc_l_tmp_enum_switch_result_n1 = 0;
  switch (hxc_l_value_h10794fed7059.hxc_tag) {
    case hxc_Chain_End:
      {
        if (hxc_l_value_h10794fed7059.hxc_tag != hxc_Chain_End)
        {
          abort();
        }
        int32_t hxc_l_tmp_enum_payload_project_n0 = hxc_l_value_h10794fed7059.hxc_payload.hxc_End.hxc_value;
        int32_t hxc_l_value_h81e07fd8e778 = hxc_l_tmp_enum_payload_project_n0;
        int32_t hxc_l_item_h867a45ae30df = hxc_l_value_h81e07fd8e778;
        hxc_l_tmp_enum_switch_result_n1 = hxc_l_item_h867a45ae30df;
        break;
      }
    case hxc_Chain_Link:
      {
        if (hxc_l_value_h10794fed7059.hxc_tag != hxc_Chain_Link)
        {
          abort();
        }
        int32_t hxc_l_tmp_enum_payload_project_n3 = hxc_l_value_h10794fed7059.hxc_payload.hxc_Link.hxc_value;
        int32_t hxc_l_value_hfb282a085de7 = hxc_l_tmp_enum_payload_project_n3;
        if (hxc_l_value_h10794fed7059.hxc_tag != hxc_Chain_Link)
        {
          abort();
        }
        struct hxc_Chain *hxc_l_tmp_enum_payload_project_n4 = hxc_l_value_h10794fed7059.hxc_payload.hxc_Link.hxc_next;
        struct hxc_Chain hxc_l_tmp_enum_recursive_payload_load_result_n5 = *hxc_l_tmp_enum_payload_project_n4;
        hxc_l_next_hcce74b90370b = hxc_l_tmp_enum_recursive_payload_load_result_n5;
        if (hxc_enum_39285fe9_retain(&hxc_l_next_hcce74b90370b) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_item_h6d1bff9ec5ac = hxc_l_value_hfb282a085de7;
        hxc_l_next_h24eca731d2de = hxc_l_next_hcce74b90370b;
        if (hxc_enum_39285fe9_retain(&hxc_l_next_h24eca731d2de) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_load_result_n8 = hxc_l_item_h6d1bff9ec5ac;
        int32_t hxc_l_tmp_call_result_n10 = hxc_EnumFixture_tailValue(hxc_l_next_h24eca731d2de);
        hxc_l_tmp_enum_switch_result_n1 = hxc_i32_add_wrapping(hxc_l_tmp_load_result_n8, hxc_l_tmp_call_result_n10);
        hxc_enum_39285fe9_destroy(&hxc_l_next_h24eca731d2de);
        hxc_enum_39285fe9_destroy(&hxc_l_next_hcce74b90370b);
        break;
      }
    default:
      {
        abort();
      }
  }
  return hxc_l_tmp_enum_switch_result_n1;
}

int32_t hxc_EnumFixture_choiceValue(struct hxc_Choices hxc_l_value)
{
  struct hxc_array_ref *hxc_l_items = { 0 };
  int32_t hxc_l_tmp_enum_switch_result_n1 = 0;
  switch (hxc_l_value.hxc_tag) {
    case hxc_Choices_NoChoices:
      {
        hxc_l_tmp_enum_switch_result_n1 = 0;
        break;
      }
    case hxc_Choices_ChoiceValues:
      {
        if (hxc_l_value.hxc_tag != hxc_Choices_ChoiceValues)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_enum_payload_project_n0 = hxc_l_value.hxc_payload.hxc_ChoiceValues.hxc_values;
        struct hxc_array_ref *hxc_l_values = hxc_l_tmp_enum_payload_project_n0;
        hxc_l_items = hxc_l_values;
        if (hxc_array_ref_retain(hxc_l_items) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_array_get_result_n3;
        if (hxc_array_ref_get_copy(hxc_l_items, (size_t)0, &hxc_l_tmp_array_get_result_n3) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_enum_switch_result_n1 = hxc_l_tmp_array_get_result_n3;
        if (hxc_array_ref_release(hxc_l_items) != HXC_STATUS_OK)
        {
          abort();
        }
        break;
      }
    default:
      {
        abort();
      }
  }
  return hxc_l_tmp_enum_switch_result_n1;
}

int32_t hxc_EnumFixture_constructorValue(void)
{
  struct hxc_Option_h95f1c4a28dac hxc_l_tmp_call_result_n1 = hxc_EnumFixture_applyOption(9, (struct hxc_EnumFixture_StackClosure){ .hxc_invoke = hxc_Option_i32_Some_synchronous_callback_adapter, .hxc_context = NULL });
  int32_t hxc_l_tmp_call_result_n2 = hxc_EnumFixture_optionValue(hxc_l_tmp_call_result_n1);
  return hxc_l_tmp_call_result_n2;
}

struct hxc_RuleEnvelope hxc_EnumFixture_copyEnvelope(struct hxc_RuleEnvelope hxc_l_value)
{
  struct hxc_RuleEnvelope hxc_l_tmp_returned_enum_owner_n1 = hxc_l_value;
  if (hxc_enum_ffce8027_retain(&hxc_l_tmp_returned_enum_owner_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_returned_enum_owner_n1;
}

struct hxc_Rule hxc_EnumFixture_copyRule(struct hxc_Rule hxc_l_value)
{
  struct hxc_Rule hxc_l_tmp_returned_record_owner_n1 = hxc_l_value;
  if (hxc_record_9f230b68_retain(&hxc_l_tmp_returned_record_owner_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_returned_record_owner_n1;
}

bool hxc_EnumFixture_envelopeIsWrapped(struct hxc_RuleEnvelope hxc_l_value)
{
  if (hxc_l_value.hxc_tag == hxc_RuleEnvelope_WrappedRule)
  {
    return true;
  }
  return false;
}

struct hxc_array_ref *hxc_EnumFixture_envelopeLiteral(struct hxc_Rule hxc_l_fresh, struct hxc_RuleEnvelope hxc_l_borrowed)
{
  struct hxc_Rule hxc_l_tmp_enum_payload_0_owner_n2 = hxc_l_fresh;
  if (hxc_record_9f230b68_retain(&hxc_l_tmp_enum_payload_0_owner_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_RuleEnvelope hxc_l_tmp_array_literal_element_0_owner_n3 = (struct hxc_RuleEnvelope){ .hxc_tag = hxc_RuleEnvelope_WrappedRule, .hxc_payload.hxc_WrappedRule.hxc_rule = hxc_l_tmp_enum_payload_0_owner_n2 };
  struct hxc_RuleEnvelope hxc_l_tmp_array_literal_element_0_borrow_result_n2 = hxc_l_tmp_array_literal_element_0_owner_n3;
  struct hxc_RuleEnvelope hxc_l_tmp_array_literal_element_2_owner_n4 = (struct hxc_RuleEnvelope){ .hxc_tag = hxc_RuleEnvelope_MissingRule };
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n5 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_RuleEnvelope), _Alignof(struct hxc_RuleEnvelope), NULL, hxc_array_84c38722_element_copy, hxc_array_84c38722_element_assign, hxc_array_84c38722_element_destroy }, &hxc_l_tmp_array_create_result_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n5->value, &hxc_l_tmp_array_literal_element_0_borrow_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n5->value, &hxc_l_borrowed) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n5->value, &hxc_l_tmp_array_literal_element_2_owner_n4) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_enum_ffce8027_destroy(&hxc_l_tmp_array_literal_element_2_owner_n4);
  hxc_enum_ffce8027_destroy(&hxc_l_tmp_array_literal_element_0_owner_n3);
  return hxc_l_tmp_array_create_result_n5;
}

int32_t hxc_EnumFixture_envelopeValue(struct hxc_RuleEnvelope hxc_l_value)
{
  struct hxc_Rule hxc_l_rule_h5227d8af703a = { 0 };
  struct hxc_Rule hxc_l_rule_hefedbce21b8f = { 0 };
  int32_t hxc_l_tmp_enum_switch_result_n1 = 0;
  switch (hxc_l_value.hxc_tag) {
    case hxc_RuleEnvelope_MissingRule:
      {
        hxc_l_tmp_enum_switch_result_n1 = 0;
        break;
      }
    case hxc_RuleEnvelope_WrappedRule:
      {
        if (hxc_l_value.hxc_tag != hxc_RuleEnvelope_WrappedRule)
        {
          abort();
        }
        struct hxc_Rule hxc_l_tmp_enum_payload_project_n0 = hxc_l_value.hxc_payload.hxc_WrappedRule.hxc_rule;
        hxc_l_rule_h5227d8af703a = hxc_l_tmp_enum_payload_project_n0;
        if (hxc_record_9f230b68_retain(&hxc_l_rule_h5227d8af703a) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_rule_hefedbce21b8f = hxc_l_rule_h5227d8af703a;
        if (hxc_record_9f230b68_retain(&hxc_l_rule_hefedbce21b8f) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_call_result_n3 = hxc_EnumFixture_ruleValue(hxc_l_rule_hefedbce21b8f);
        hxc_l_tmp_enum_switch_result_n1 = hxc_l_tmp_call_result_n3;
        hxc_record_9f230b68_destroy(&hxc_l_rule_hefedbce21b8f);
        hxc_record_9f230b68_destroy(&hxc_l_rule_h5227d8af703a);
        break;
      }
    default:
      {
        abort();
      }
  }
  return hxc_l_tmp_enum_switch_result_n1;
}

int32_t hxc_EnumFixture_guardedValue(struct hxc_Option_h95f1c4a28dac hxc_l_value_ha201421511a7)
{
  int32_t hxc_l_tmp_enum_switch_result_n1 = 0;
  switch (hxc_l_value_ha201421511a7.hxc_tag) {
    case hxc_Option_None_h506b5e6013bd:
      {
        hxc_l_tmp_enum_switch_result_n1 = -1;
        break;
      }
    case hxc_Option_Some_ha9454146ff01:
      {
        if (hxc_l_value_ha201421511a7.hxc_tag != hxc_Option_Some_ha9454146ff01)
        {
          abort();
        }
        int32_t hxc_l_tmp_enum_payload_project_n0 = hxc_l_value_ha201421511a7.hxc_payload.hxc_Some.hxc_value;
        int32_t hxc_l_value_h8792d0ecd498 = hxc_l_tmp_enum_payload_project_n0;
        int32_t hxc_l_payload_h6e5ec5c1b4a8 = hxc_l_value_h8792d0ecd498;
        int32_t hxc_l_tmp_load_result_n2 = hxc_l_payload_h6e5ec5c1b4a8;
        int32_t hxc_l_tmp_conditional_result_n4 = 0;
        if (hxc_l_tmp_load_result_n2 > 4)
        {
          hxc_l_tmp_conditional_result_n4 = hxc_l_payload_h6e5ec5c1b4a8;
        }
        else
        {
          int32_t hxc_l_payload_h0b6a49bc2290 = hxc_l_value_h8792d0ecd498;
          hxc_l_tmp_conditional_result_n4 = hxc_i32_add_wrapping(hxc_l_payload_h0b6a49bc2290, 1);
        }
        hxc_l_tmp_enum_switch_result_n1 = hxc_l_tmp_conditional_result_n4;
        break;
      }
    default:
      {
        abort();
      }
  }
  return hxc_l_tmp_enum_switch_result_n1;
}

int32_t hxc_EnumFixture_identity(int32_t hxc_l_value)
{
  return hxc_l_value;
}

enum hxc_Mode hxc_EnumFixture_identityMode(enum hxc_Mode hxc_l_value)
{
  return hxc_l_value;
}

void hxc_EnumFixture_main(void)
{
  const void *volatile hxc_l_gc_roots[2] = { NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 2, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_Chain hxc_l_tmp_static_call_argument_0_owner_n39 = { 0 };
  struct hxc_Choices hxc_l_tmp_static_call_argument_1_owner_n41 = { 0 };
  enum hxc_Mode hxc_l_mode = hxc_Mode_On;
  int32_t hxc_l_tmp_call_result_n1 = hxc_EnumFixture_identity(7);
  struct hxc_Option_h95f1c4a28dac hxc_l_present = (struct hxc_Option_h95f1c4a28dac){ .hxc_tag = hxc_Option_Some_ha9454146ff01, .hxc_payload.hxc_Some.hxc_value = hxc_l_tmp_call_result_n1 };
  struct hxc_Option_h95f1c4a28dac hxc_l_absent = (struct hxc_Option_h95f1c4a28dac){ .hxc_tag = hxc_Option_None_h506b5e6013bd };
  struct hxc_Option_ha0e4b5dcc139 hxc_l_truth = (struct hxc_Option_ha0e4b5dcc139){ .hxc_tag = hxc_Option_Some_h33493695ace2, .hxc_payload.hxc_Some.hxc_value = true };
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n5 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(int32_t), _Alignof(int32_t), &hxc_l_tmp_array_create_result_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n5->value, &(int32_t){ 3 }) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_choices = hxc_l_tmp_array_create_result_n5;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n6 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(int32_t), _Alignof(int32_t), &hxc_l_tmp_array_create_result_n6) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n6->value, &(int32_t){ 4 }) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_actions = hxc_l_tmp_array_create_result_n6;
  struct hxc_Chain *hxc_l_tmp_enum_recursive_payload_owner_n8 = NULL;
  hxc_allocator hxc_l_tmp_enum_recursive_payload_owner_n8_allocator = hxc_default_allocator();
  if (hxc_alloc(&hxc_l_tmp_enum_recursive_payload_owner_n8_allocator, sizeof(struct hxc_Chain), _Alignof(struct hxc_Chain), (void **)&hxc_l_tmp_enum_recursive_payload_owner_n8) != HXC_STATUS_OK)
  {
    abort();
  }
  *hxc_l_tmp_enum_recursive_payload_owner_n8 = (struct hxc_Chain){ .hxc_tag = hxc_Chain_End, .hxc_payload.hxc_End.hxc_value = 2 };
  struct hxc_Chain hxc_l_tmp_static_call_argument_0_owner_n7 = (struct hxc_Chain){ .hxc_tag = hxc_Chain_Link, .hxc_payload.hxc_Link.hxc_value = 1, .hxc_payload.hxc_Link.hxc_next = hxc_l_tmp_enum_recursive_payload_owner_n8 };
  struct hxc_Chain hxc_l_tmp_static_call_argument_0_borrow_result_n10 = hxc_l_tmp_static_call_argument_0_owner_n7;
  struct hxc_array_ref *hxc_l_tmp_enum_payload_0_owner_n8 = hxc_l_choices;
  if (hxc_array_ref_retain(hxc_l_tmp_enum_payload_0_owner_n8) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_Choices hxc_l_tmp_static_call_argument_1_owner_n9 = (struct hxc_Choices){ .hxc_tag = hxc_Choices_ChoiceValues, .hxc_payload.hxc_ChoiceValues.hxc_values = hxc_l_tmp_enum_payload_0_owner_n8 };
  struct hxc_Choices hxc_l_tmp_static_call_argument_1_borrow_result_n14 = hxc_l_tmp_static_call_argument_1_owner_n9;
  struct hxc_Rule hxc_l_tmp_call_result_n16 = hxc_EnumFixture_makeRule(hxc_l_tmp_static_call_argument_0_borrow_result_n10, hxc_l_tmp_static_call_argument_1_borrow_result_n14, hxc_l_actions);
  struct hxc_Rule hxc_l_rule = hxc_l_tmp_call_result_n16;
  struct hxc_Rule hxc_l_tmp_call_result_n18 = hxc_EnumFixture_copyRule(hxc_l_rule);
  struct hxc_Rule hxc_l_copiedRule = hxc_l_tmp_call_result_n18;
  struct hxc_RuleEnvelope hxc_l_tmp_call_result_n20 = hxc_EnumFixture_wrapRule(hxc_l_copiedRule);
  struct hxc_RuleEnvelope hxc_l_envelope = hxc_l_tmp_call_result_n20;
  struct hxc_RuleEnvelope hxc_l_tmp_call_result_n22 = hxc_EnumFixture_copyEnvelope(hxc_l_envelope);
  struct hxc_RuleEnvelope hxc_l_copiedEnvelope = hxc_l_tmp_call_result_n22;
  struct hxc_Rule hxc_l_tmp_enum_payload_0_owner_n14 = hxc_l_copiedRule;
  if (hxc_record_9f230b68_retain(&hxc_l_tmp_enum_payload_0_owner_n14) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_Option_h2a07afaff02e hxc_l_optionalRule = (struct hxc_Option_h2a07afaff02e){ .hxc_tag = hxc_Option_Some_ha8dd5a59e40a, .hxc_payload.hxc_Some.hxc_value = hxc_l_tmp_enum_payload_0_owner_n14 };
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n26 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_Rule), _Alignof(struct hxc_Rule), NULL, hxc_array_400559e4_element_copy, hxc_array_400559e4_element_assign, hxc_array_400559e4_element_destroy }, &hxc_l_tmp_array_create_result_n26) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_rules = hxc_l_tmp_array_create_result_n26;
  struct hxc_array_ref *hxc_l_tmp_load_result_n27 = hxc_l_rules;
  int32_t hxc_l_tmp_array_push_result_n29;
  if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n27, &hxc_l_copiedRule, &hxc_l_tmp_array_push_result_n29) != HXC_STATUS_OK)
  {
    abort();
  }
  (void)hxc_l_tmp_array_push_result_n29;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n30 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_RuleEnvelope), _Alignof(struct hxc_RuleEnvelope), NULL, hxc_array_84c38722_element_copy, hxc_array_84c38722_element_assign, hxc_array_84c38722_element_destroy }, &hxc_l_tmp_array_create_result_n30) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_envelopes = hxc_l_tmp_array_create_result_n30;
  struct hxc_array_ref *hxc_l_tmp_load_result_n31 = hxc_l_envelopes;
  int32_t hxc_l_tmp_array_push_result_n33;
  if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n31, &hxc_l_copiedEnvelope, &hxc_l_tmp_array_push_result_n33) != HXC_STATUS_OK)
  {
    abort();
  }
  (void)hxc_l_tmp_array_push_result_n33;
  struct hxc_Rule hxc_l_tmp_load_result_n34 = hxc_l_copiedRule;
  struct hxc_array_ref *hxc_l_tmp_call_result_n36 = hxc_EnumFixture_envelopeLiteral(hxc_l_tmp_load_result_n34, hxc_l_copiedEnvelope);
  struct hxc_array_ref *hxc_l_literalEnvelopes = hxc_l_tmp_call_result_n36;
  struct hxc_RecursiveActionPlan hxc_l_tmp_call_result_n37 = hxc_EnumFixture_recursiveActionPlan();
  hxc_l_gc_roots[0] = (const void *)hxc_l_tmp_call_result_n37.hxc_actions;
  struct hxc_RecursiveActionPlan hxc_l_recursivePlan = hxc_l_tmp_call_result_n37;
  while (1)
  {
    int32_t hxc_l_tmp_call_result_n39 = hxc_EnumFixture_modeValue(hxc_l_mode);
    bool hxc_l_tmp_short_circuit_result_n19 = hxc_l_tmp_call_result_n39 == 1;
    if (hxc_l_tmp_call_result_n39 == 1)
    {
      bool hxc_l_tmp_call_result_n41 = hxc_EnumFixture_modeIsOn(hxc_l_mode);
      hxc_l_tmp_short_circuit_result_n19 = hxc_l_tmp_call_result_n41;
    }
    bool hxc_l_tmp_short_circuit_load_result_n42 = hxc_l_tmp_short_circuit_result_n19;
    bool hxc_l_tmp_short_circuit_result_n20 = hxc_l_tmp_short_circuit_load_result_n42;
    if (hxc_l_tmp_short_circuit_load_result_n42)
    {
      bool hxc_l_tmp_call_result_n43 = hxc_EnumFixture_modeEquality();
      hxc_l_tmp_short_circuit_result_n20 = hxc_l_tmp_call_result_n43;
    }
    bool hxc_l_tmp_short_circuit_load_result_n44 = hxc_l_tmp_short_circuit_result_n20;
    bool hxc_l_tmp_short_circuit_result_n21 = hxc_l_tmp_short_circuit_load_result_n44;
    if (hxc_l_tmp_short_circuit_load_result_n44)
    {
      int32_t hxc_l_tmp_call_result_n46 = hxc_EnumFixture_optionValue(hxc_l_present);
      hxc_l_tmp_short_circuit_result_n21 = hxc_l_tmp_call_result_n46 == 7;
    }
    bool hxc_l_tmp_short_circuit_load_result_n47 = hxc_l_tmp_short_circuit_result_n21;
    bool hxc_l_tmp_short_circuit_result_n22 = hxc_l_tmp_short_circuit_load_result_n47;
    if (hxc_l_tmp_short_circuit_load_result_n47)
    {
      bool hxc_l_tmp_call_result_n48 = hxc_EnumFixture_optionTagEquality();
      hxc_l_tmp_short_circuit_result_n22 = hxc_l_tmp_call_result_n48;
    }
    bool hxc_l_tmp_short_circuit_load_result_n49 = hxc_l_tmp_short_circuit_result_n22;
    bool hxc_l_tmp_short_circuit_result_n23 = hxc_l_tmp_short_circuit_load_result_n49;
    if (hxc_l_tmp_short_circuit_load_result_n49)
    {
      bool hxc_l_tmp_call_result_n51 = hxc_EnumFixture_optionHasPositiveValue(hxc_l_present);
      hxc_l_tmp_short_circuit_result_n23 = hxc_l_tmp_call_result_n51;
    }
    bool hxc_l_tmp_short_circuit_load_result_n52 = hxc_l_tmp_short_circuit_result_n23;
    bool hxc_l_tmp_short_circuit_result_n24 = hxc_l_tmp_short_circuit_load_result_n52;
    if (hxc_l_tmp_short_circuit_load_result_n52)
    {
      int32_t hxc_l_tmp_call_result_n54 = hxc_EnumFixture_optionValue(hxc_l_absent);
      hxc_l_tmp_short_circuit_result_n24 = hxc_l_tmp_call_result_n54 == 0;
    }
    bool hxc_l_tmp_short_circuit_load_result_n55 = hxc_l_tmp_short_circuit_result_n24;
    bool hxc_l_tmp_short_circuit_result_n25 = hxc_l_tmp_short_circuit_load_result_n55;
    if (hxc_l_tmp_short_circuit_load_result_n55)
    {
      int32_t hxc_l_tmp_call_result_n56 = hxc_EnumFixture_constructorValue();
      hxc_l_tmp_short_circuit_result_n25 = hxc_l_tmp_call_result_n56 == 9;
    }
    bool hxc_l_tmp_short_circuit_load_result_n57 = hxc_l_tmp_short_circuit_result_n25;
    bool hxc_l_tmp_short_circuit_result_n26 = hxc_l_tmp_short_circuit_load_result_n57;
    if (hxc_l_tmp_short_circuit_load_result_n57)
    {
      int32_t hxc_l_tmp_call_result_n59 = hxc_EnumFixture_guardedValue(hxc_l_present);
      hxc_l_tmp_short_circuit_result_n26 = hxc_l_tmp_call_result_n59 == 7;
    }
    bool hxc_l_tmp_short_circuit_load_result_n60 = hxc_l_tmp_short_circuit_result_n26;
    bool hxc_l_tmp_short_circuit_result_n27 = hxc_l_tmp_short_circuit_load_result_n60;
    if (hxc_l_tmp_short_circuit_load_result_n60)
    {
      int32_t hxc_l_tmp_call_result_n62 = hxc_EnumFixture_boolOptionValue(hxc_l_truth);
      hxc_l_tmp_short_circuit_result_n27 = hxc_l_tmp_call_result_n62 == 1;
    }
    bool hxc_l_tmp_short_circuit_load_result_n63 = hxc_l_tmp_short_circuit_result_n27;
    bool hxc_l_tmp_short_circuit_result_n28 = hxc_l_tmp_short_circuit_load_result_n63;
    if (hxc_l_tmp_short_circuit_load_result_n63)
    {
      int32_t hxc_l_tmp_call_result_n66 = hxc_EnumFixture_pairedIdentityValue(hxc_IdentityKind_FirstIdentity, (struct hxc_IdentityValue){ .hxc_tag = hxc_IdentityValue_FirstValue, .hxc_payload.hxc_FirstValue.hxc_value = 12 });
      hxc_l_tmp_short_circuit_result_n28 = hxc_l_tmp_call_result_n66 == 12;
    }
    bool hxc_l_tmp_short_circuit_load_result_n67 = hxc_l_tmp_short_circuit_result_n28;
    bool hxc_l_tmp_short_circuit_result_n29 = hxc_l_tmp_short_circuit_load_result_n67;
    if (hxc_l_tmp_short_circuit_load_result_n67)
    {
      int32_t hxc_l_tmp_call_result_n70 = hxc_EnumFixture_pairedIdentityValue(hxc_IdentityKind_FirstIdentity, (struct hxc_IdentityValue){ .hxc_tag = hxc_IdentityValue_SecondValue, .hxc_payload.hxc_SecondValue.hxc_value = 12 });
      hxc_l_tmp_short_circuit_result_n29 = hxc_l_tmp_call_result_n70 == -1;
    }
    bool hxc_l_tmp_short_circuit_load_result_n71 = hxc_l_tmp_short_circuit_result_n29;
    bool hxc_l_tmp_short_circuit_result_n30 = hxc_l_tmp_short_circuit_load_result_n71;
    if (hxc_l_tmp_short_circuit_load_result_n71)
    {
      int32_t hxc_l_tmp_call_result_n74 = hxc_EnumFixture_pairedIdentityValue(hxc_IdentityKind_SecondIdentity, (struct hxc_IdentityValue){ .hxc_tag = hxc_IdentityValue_SecondValue, .hxc_payload.hxc_SecondValue.hxc_value = 14 });
      hxc_l_tmp_short_circuit_result_n30 = hxc_l_tmp_call_result_n74 == 14;
    }
    bool hxc_l_tmp_short_circuit_load_result_n75 = hxc_l_tmp_short_circuit_result_n30;
    bool hxc_l_tmp_short_circuit_result_n31 = hxc_l_tmp_short_circuit_load_result_n75;
    if (hxc_l_tmp_short_circuit_load_result_n75)
    {
      struct hxc_StrictCarrierHolder hxc_l_tmp_call_result_n77 = hxc_EnumFixture_strictCarrierHolder(hxc_IdentityKind_FirstIdentity);
      int32_t hxc_l_tmp_call_result_n78 = hxc_EnumFixture_strictCarrierValue(hxc_l_tmp_call_result_n77);
      hxc_l_tmp_short_circuit_result_n31 = hxc_l_tmp_call_result_n78 == 0;
    }
    bool hxc_l_tmp_short_circuit_load_result_n79 = hxc_l_tmp_short_circuit_result_n31;
    bool hxc_l_tmp_short_circuit_result_n32 = hxc_l_tmp_short_circuit_load_result_n79;
    if (hxc_l_tmp_short_circuit_load_result_n79)
    {
      struct hxc_StrictCarrierHolder hxc_l_tmp_call_result_n81 = hxc_EnumFixture_strictCarrierHolder(hxc_IdentityKind_SecondIdentity);
      int32_t hxc_l_tmp_call_result_n82 = hxc_EnumFixture_strictCarrierValue(hxc_l_tmp_call_result_n81);
      hxc_l_tmp_short_circuit_result_n32 = hxc_l_tmp_call_result_n82 == 23;
    }
    bool hxc_l_tmp_short_circuit_load_result_n83 = hxc_l_tmp_short_circuit_result_n32;
    bool hxc_l_tmp_short_circuit_result_n33 = hxc_l_tmp_short_circuit_load_result_n83;
    if (hxc_l_tmp_short_circuit_load_result_n83)
    {
      int32_t hxc_l_tmp_call_result_n84 = hxc_EnumFixture_recursiveLocal();
      hxc_l_tmp_short_circuit_result_n33 = hxc_l_tmp_call_result_n84 == 3;
    }
    bool hxc_l_tmp_short_circuit_load_result_n85 = hxc_l_tmp_short_circuit_result_n33;
    bool hxc_l_tmp_short_circuit_result_n34 = hxc_l_tmp_short_circuit_load_result_n85;
    if (hxc_l_tmp_short_circuit_load_result_n85)
    {
      int32_t hxc_l_tmp_call_result_n87 = hxc_EnumFixture_ruleValue(hxc_l_copiedRule);
      hxc_l_tmp_short_circuit_result_n34 = hxc_l_tmp_call_result_n87 == 10;
    }
    bool hxc_l_tmp_short_circuit_load_result_n88 = hxc_l_tmp_short_circuit_result_n34;
    bool hxc_l_tmp_short_circuit_result_n35 = hxc_l_tmp_short_circuit_load_result_n88;
    if (hxc_l_tmp_short_circuit_load_result_n88)
    {
      int32_t hxc_l_tmp_call_result_n90 = hxc_EnumFixture_envelopeValue(hxc_l_copiedEnvelope);
      hxc_l_tmp_short_circuit_result_n35 = hxc_l_tmp_call_result_n90 == 10;
    }
    bool hxc_l_tmp_short_circuit_load_result_n91 = hxc_l_tmp_short_circuit_result_n35;
    bool hxc_l_tmp_short_circuit_result_n36 = hxc_l_tmp_short_circuit_load_result_n91;
    if (hxc_l_tmp_short_circuit_load_result_n91)
    {
      bool hxc_l_tmp_call_result_n93 = hxc_EnumFixture_envelopeIsWrapped(hxc_l_copiedEnvelope);
      hxc_l_tmp_short_circuit_result_n36 = hxc_l_tmp_call_result_n93;
    }
    bool hxc_l_tmp_short_circuit_load_result_n94 = hxc_l_tmp_short_circuit_result_n36;
    bool hxc_l_tmp_short_circuit_result_n37 = hxc_l_tmp_short_circuit_load_result_n94;
    if (hxc_l_tmp_short_circuit_load_result_n94)
    {
      int32_t hxc_l_tmp_call_result_n96 = hxc_EnumFixture_optionalRuleValue(hxc_l_optionalRule);
      hxc_l_tmp_short_circuit_result_n37 = hxc_l_tmp_call_result_n96 == 10;
    }
    bool hxc_l_tmp_short_circuit_load_result_n97 = hxc_l_tmp_short_circuit_result_n37;
    bool hxc_l_tmp_short_circuit_result_n38 = hxc_l_tmp_short_circuit_load_result_n97;
    if (hxc_l_tmp_short_circuit_load_result_n97)
    {
      struct hxc_Chain *hxc_l_tmp_enum_recursive_payload_owner_n99 = NULL;
      hxc_allocator hxc_l_tmp_enum_recursive_payload_owner_n99_allocator = hxc_default_allocator();
      if (hxc_alloc(&hxc_l_tmp_enum_recursive_payload_owner_n99_allocator, sizeof(struct hxc_Chain), _Alignof(struct hxc_Chain), (void **)&hxc_l_tmp_enum_recursive_payload_owner_n99) != HXC_STATUS_OK)
      {
        abort();
      }
      *hxc_l_tmp_enum_recursive_payload_owner_n99 = (struct hxc_Chain){ .hxc_tag = hxc_Chain_End, .hxc_payload.hxc_End.hxc_value = 2 };
      hxc_l_tmp_static_call_argument_0_owner_n39 = (struct hxc_Chain){ .hxc_tag = hxc_Chain_Link, .hxc_payload.hxc_Link.hxc_value = 1, .hxc_payload.hxc_Link.hxc_next = hxc_l_tmp_enum_recursive_payload_owner_n99 };
      struct hxc_Chain hxc_l_tmp_static_call_argument_0_borrow_result_n101 = hxc_l_tmp_static_call_argument_0_owner_n39;
      struct hxc_array_ref *hxc_l_tmp_enum_payload_0_owner_n40 = hxc_l_choices;
      if (hxc_array_ref_retain(hxc_l_tmp_enum_payload_0_owner_n40) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_static_call_argument_1_owner_n41 = (struct hxc_Choices){ .hxc_tag = hxc_Choices_ChoiceValues, .hxc_payload.hxc_ChoiceValues.hxc_values = hxc_l_tmp_enum_payload_0_owner_n40 };
      struct hxc_Choices hxc_l_tmp_static_call_argument_1_borrow_result_n105 = hxc_l_tmp_static_call_argument_1_owner_n41;
      struct hxc_array_ref *hxc_l_tmp_load_result_n106 = hxc_l_actions;
      int32_t hxc_l_tmp_call_result_n108 = hxc_EnumFixture_ruleLiteralValue(hxc_l_tmp_static_call_argument_0_borrow_result_n101, hxc_l_tmp_static_call_argument_1_borrow_result_n105, hxc_l_tmp_load_result_n106, hxc_l_copiedRule);
      hxc_l_tmp_short_circuit_result_n38 = hxc_l_tmp_call_result_n108 == 12;
      hxc_enum_d215f611_destroy(&hxc_l_tmp_static_call_argument_1_owner_n41);
      hxc_enum_39285fe9_destroy(&hxc_l_tmp_static_call_argument_0_owner_n39);
    }
    bool hxc_l_tmp_short_circuit_load_result_n109 = hxc_l_tmp_short_circuit_result_n38;
    bool hxc_l_tmp_short_circuit_result_n42 = hxc_l_tmp_short_circuit_load_result_n109;
    if (hxc_l_tmp_short_circuit_load_result_n109)
    {
      int32_t hxc_l_tmp_array_length_result_n111;
      if (hxc_array_ref_length(hxc_l_envelopes, &hxc_l_tmp_array_length_result_n111) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_short_circuit_result_n42 = hxc_l_tmp_array_length_result_n111 == 1;
    }
    bool hxc_l_tmp_short_circuit_load_result_n112 = hxc_l_tmp_short_circuit_result_n42;
    bool hxc_l_tmp_short_circuit_result_n43 = hxc_l_tmp_short_circuit_load_result_n112;
    if (hxc_l_tmp_short_circuit_load_result_n112)
    {
      int32_t hxc_l_tmp_array_length_result_n114;
      if (hxc_array_ref_length(hxc_l_literalEnvelopes, &hxc_l_tmp_array_length_result_n114) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_short_circuit_result_n43 = hxc_l_tmp_array_length_result_n114 == 3;
    }
    bool hxc_l_tmp_short_circuit_load_result_n115 = hxc_l_tmp_short_circuit_result_n43;
    bool hxc_l_tmp_short_circuit_result_n44 = hxc_l_tmp_short_circuit_load_result_n115;
    if (hxc_l_tmp_short_circuit_load_result_n115)
    {
      hxc_l_gc_roots[1] = (const void *)hxc_l_recursivePlan.hxc_actions;
      int32_t hxc_l_tmp_call_result_n117 = hxc_EnumFixture_recursiveActionPlanValue(hxc_l_recursivePlan);
      hxc_l_tmp_short_circuit_result_n44 = hxc_l_tmp_call_result_n117 == 17;
    }
    bool hxc_l_tmp_short_circuit_load_result_n118 = hxc_l_tmp_short_circuit_result_n44;
    bool hxc_l_tmp_short_circuit_result_n45 = hxc_l_tmp_short_circuit_load_result_n118;
    if (hxc_l_tmp_short_circuit_load_result_n118)
    {
      int32_t hxc_l_tmp_array_length_result_n120;
      if (hxc_array_ref_length(hxc_l_rules, &hxc_l_tmp_array_length_result_n120) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_short_circuit_result_n45 = hxc_l_tmp_array_length_result_n120 == 1;
    }
    if (!!hxc_l_tmp_short_circuit_result_n45)
    {
      break;
    }
  }
  if (hxc_array_ref_release(hxc_l_literalEnvelopes) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_envelopes) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_rules) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_enum_24936704_destroy(&hxc_l_optionalRule);
  hxc_enum_ffce8027_destroy(&hxc_l_copiedEnvelope);
  hxc_enum_ffce8027_destroy(&hxc_l_envelope);
  hxc_record_9f230b68_destroy(&hxc_l_copiedRule);
  hxc_record_9f230b68_destroy(&hxc_l_rule);
  hxc_enum_d215f611_destroy(&hxc_l_tmp_static_call_argument_1_owner_n9);
  hxc_enum_39285fe9_destroy(&hxc_l_tmp_static_call_argument_0_owner_n7);
  if (hxc_array_ref_release(hxc_l_actions) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_choices) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

struct hxc_Rule hxc_EnumFixture_makeRule(struct hxc_Chain hxc_l_chain, struct hxc_Choices hxc_l_choices, struct hxc_array_ref *hxc_l_actions)
{
  struct hxc_Chain hxc_l_tmp_record_field_chain_owner_n3 = hxc_l_chain;
  if (hxc_enum_39285fe9_retain(&hxc_l_tmp_record_field_chain_owner_n3) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_Chain hxc_l_tmp_record_field_chain_owned_load_result_n0 = hxc_l_tmp_record_field_chain_owner_n3;
  struct hxc_Choices hxc_l_tmp_record_field_choices_owner_n4 = hxc_l_choices;
  if (hxc_enum_d215f611_retain(&hxc_l_tmp_record_field_choices_owner_n4) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_Choices hxc_l_tmp_record_field_choices_owned_load_result_n1 = hxc_l_tmp_record_field_choices_owner_n4;
  struct hxc_array_ref *hxc_l_tmp_record_field_actions_owner_n5 = hxc_l_actions;
  if (hxc_array_ref_retain(hxc_l_tmp_record_field_actions_owner_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_Rule){ .hxc_actions = hxc_l_tmp_record_field_actions_owner_n5, .hxc_chain = hxc_l_tmp_record_field_chain_owned_load_result_n0, .hxc_choices = hxc_l_tmp_record_field_choices_owned_load_result_n1 };
}

bool hxc_EnumFixture_modeEquality(void)
{
  enum hxc_Mode hxc_l_tmp_call_result_n1 = hxc_EnumFixture_identityMode(hxc_Mode_On);
  enum hxc_Mode hxc_l_tmp_call_result_n3 = hxc_EnumFixture_identityMode(hxc_Mode_On);
  bool hxc_l_same = hxc_l_tmp_call_result_n1 == hxc_l_tmp_call_result_n3;
  enum hxc_Mode hxc_l_tmp_call_result_n5 = hxc_EnumFixture_identityMode(hxc_Mode_Off);
  enum hxc_Mode hxc_l_tmp_call_result_n7 = hxc_EnumFixture_identityMode(hxc_Mode_On);
  bool hxc_l_different = hxc_l_tmp_call_result_n5 != hxc_l_tmp_call_result_n7;
  bool hxc_l_tmp_load_result_n8 = hxc_l_same;
  bool hxc_l_tmp_short_circuit_result_n2 = hxc_l_tmp_load_result_n8;
  if (hxc_l_tmp_load_result_n8)
  {
    hxc_l_tmp_short_circuit_result_n2 = hxc_l_different;
  }
  return hxc_l_tmp_short_circuit_result_n2;
}

bool hxc_EnumFixture_modeIsOn(enum hxc_Mode hxc_l_value)
{
  if (hxc_l_value == hxc_Mode_On)
  {
    return true;
  }
  return false;
}

int32_t hxc_EnumFixture_modeValue(enum hxc_Mode hxc_l_value)
{
  int32_t hxc_l_tmp_enum_switch_result_n1 = 0;
  switch (hxc_l_value) {
    case hxc_Mode_Off:
      {
        hxc_l_tmp_enum_switch_result_n1 = 0;
        break;
      }
    case hxc_Mode_On:
      {
        hxc_l_tmp_enum_switch_result_n1 = 1;
        break;
      }
    default:
      {
        abort();
      }
  }
  return hxc_l_tmp_enum_switch_result_n1;
}

struct hxc_Option_h95f1c4a28dac hxc_EnumFixture_observedOption(struct hxc_Option_h95f1c4a28dac hxc_l_value, struct hxc_array_ref *hxc_l_evaluations)
{
  int32_t hxc_l_tmp_array_update_old_result_n0;
  if (hxc_array_ref_get_copy(hxc_l_evaluations, (size_t)0, &hxc_l_tmp_array_update_old_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_set_copy(hxc_l_evaluations, (size_t)0, &(int32_t){ hxc_i32_add_wrapping(hxc_l_tmp_array_update_old_result_n0, 1) }) != HXC_STATUS_OK)
  {
    abort();
  }
  (void)hxc_i32_add_wrapping(hxc_l_tmp_array_update_old_result_n0, 1);
  return hxc_l_value;
}

bool hxc_EnumFixture_optionHasPositiveValue(struct hxc_Option_h95f1c4a28dac hxc_l_value_he8fa941d9290)
{
  int32_t hxc_l_value_hd7907d0ee1b8 = { 0 };
  int32_t hxc_l_payload = { 0 };
  if (hxc_l_value_he8fa941d9290.hxc_tag == hxc_Option_Some_ha9454146ff01)
  {
    if (hxc_l_value_he8fa941d9290.hxc_tag != hxc_Option_Some_ha9454146ff01)
    {
      abort();
    }
    int32_t hxc_l_tmp_enum_payload_project_n0 = hxc_l_value_he8fa941d9290.hxc_payload.hxc_Some.hxc_value;
    hxc_l_value_hd7907d0ee1b8 = hxc_l_tmp_enum_payload_project_n0;
    hxc_l_payload = hxc_l_value_hd7907d0ee1b8;
    return hxc_l_payload > 0;
  }
  return false;
}

bool hxc_EnumFixture_optionTagEquality(void)
{
  struct hxc_Option_h95f1c4a28dac hxc_l_empty = (struct hxc_Option_h95f1c4a28dac){ .hxc_tag = hxc_Option_None_h506b5e6013bd };
  struct hxc_Option_h95f1c4a28dac hxc_l_present = (struct hxc_Option_h95f1c4a28dac){ .hxc_tag = hxc_Option_Some_ha9454146ff01, .hxc_payload.hxc_Some.hxc_value = 4 };
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n2 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(int32_t), _Alignof(int32_t), &hxc_l_tmp_array_create_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n2->value, &(int32_t){ 0 }) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_evaluations = hxc_l_tmp_array_create_result_n2;
  bool hxc_l_tmp_materialized_value_n21 = hxc_l_empty.hxc_tag == hxc_Option_None_h506b5e6013bd;
  bool hxc_l_tmp_short_circuit_result_n3 = hxc_l_tmp_materialized_value_n21;
  if (hxc_l_tmp_materialized_value_n21)
  {
    hxc_l_tmp_short_circuit_result_n3 = hxc_l_empty.hxc_tag == hxc_Option_None_h506b5e6013bd;
  }
  bool hxc_l_tmp_short_circuit_load_result_n5 = hxc_l_tmp_short_circuit_result_n3;
  bool hxc_l_tmp_short_circuit_result_n4 = hxc_l_tmp_short_circuit_load_result_n5;
  if (hxc_l_tmp_short_circuit_load_result_n5)
  {
    hxc_l_tmp_short_circuit_result_n4 = !(hxc_l_present.hxc_tag == hxc_Option_None_h506b5e6013bd);
  }
  bool hxc_l_tmp_short_circuit_load_result_n7 = hxc_l_tmp_short_circuit_result_n4;
  bool hxc_l_tmp_short_circuit_result_n5 = hxc_l_tmp_short_circuit_load_result_n7;
  if (hxc_l_tmp_short_circuit_load_result_n7)
  {
    hxc_l_tmp_short_circuit_result_n5 = !(hxc_l_present.hxc_tag == hxc_Option_None_h506b5e6013bd);
  }
  bool hxc_l_tmp_short_circuit_load_result_n9 = hxc_l_tmp_short_circuit_result_n5;
  bool hxc_l_tmp_short_circuit_result_n6 = hxc_l_tmp_short_circuit_load_result_n9;
  if (hxc_l_tmp_short_circuit_load_result_n9)
  {
    struct hxc_Option_h95f1c4a28dac hxc_l_tmp_call_result_n12 = hxc_EnumFixture_observedOption((struct hxc_Option_h95f1c4a28dac){ .hxc_tag = hxc_Option_Some_ha9454146ff01, .hxc_payload.hxc_Some.hxc_value = 5 }, hxc_l_evaluations);
    hxc_l_tmp_short_circuit_result_n6 = !(hxc_l_tmp_call_result_n12.hxc_tag == hxc_Option_None_h506b5e6013bd);
  }
  bool hxc_l_tmp_short_circuit_load_result_n13 = hxc_l_tmp_short_circuit_result_n6;
  bool hxc_l_tmp_short_circuit_result_n7 = hxc_l_tmp_short_circuit_load_result_n13;
  if (hxc_l_tmp_short_circuit_load_result_n13)
  {
    struct hxc_Option_h95f1c4a28dac hxc_l_tmp_call_result_n16 = hxc_EnumFixture_observedOption((struct hxc_Option_h95f1c4a28dac){ .hxc_tag = hxc_Option_Some_ha9454146ff01, .hxc_payload.hxc_Some.hxc_value = 6 }, hxc_l_evaluations);
    hxc_l_tmp_short_circuit_result_n7 = !(hxc_l_tmp_call_result_n16.hxc_tag == hxc_Option_None_h506b5e6013bd);
  }
  bool hxc_l_tmp_short_circuit_load_result_n17 = hxc_l_tmp_short_circuit_result_n7;
  bool hxc_l_tmp_short_circuit_result_n8 = hxc_l_tmp_short_circuit_load_result_n17;
  if (hxc_l_tmp_short_circuit_load_result_n17)
  {
    int32_t hxc_l_tmp_array_get_result_n19;
    if (hxc_array_ref_get_copy(hxc_l_evaluations, (size_t)0, &hxc_l_tmp_array_get_result_n19) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n8 = hxc_l_tmp_array_get_result_n19 == 2;
  }
  bool hxc_l_tmp_short_circuit_load_result_n20 = hxc_l_tmp_short_circuit_result_n8;
  if (hxc_array_ref_release(hxc_l_evaluations) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_short_circuit_load_result_n20;
}

int32_t hxc_EnumFixture_optionValue(struct hxc_Option_h95f1c4a28dac hxc_l_value_h2c5c76013588)
{
  int32_t hxc_l_tmp_enum_switch_result_n1 = 0;
  switch (hxc_l_value_h2c5c76013588.hxc_tag) {
    case hxc_Option_None_h506b5e6013bd:
      {
        hxc_l_tmp_enum_switch_result_n1 = 0;
        break;
      }
    case hxc_Option_Some_ha9454146ff01:
      {
        if (hxc_l_value_h2c5c76013588.hxc_tag != hxc_Option_Some_ha9454146ff01)
        {
          abort();
        }
        int32_t hxc_l_tmp_enum_payload_project_n0 = hxc_l_value_h2c5c76013588.hxc_payload.hxc_Some.hxc_value;
        int32_t hxc_l_value_h37075a18294f = hxc_l_tmp_enum_payload_project_n0;
        int32_t hxc_l_payload = hxc_l_value_h37075a18294f;
        hxc_l_tmp_enum_switch_result_n1 = hxc_l_payload;
        break;
      }
    default:
      {
        abort();
      }
  }
  return hxc_l_tmp_enum_switch_result_n1;
}

int32_t hxc_EnumFixture_optionalRuleValue(struct hxc_Option_h2a07afaff02e hxc_l_value_hffb395be3233)
{
  struct hxc_Rule hxc_l_value_hcf71bd05628a = { 0 };
  struct hxc_Rule hxc_l_rule = { 0 };
  int32_t hxc_l_tmp_enum_switch_result_n1 = 0;
  switch (hxc_l_value_hffb395be3233.hxc_tag) {
    case hxc_Option_None_hdcfb48028a4b:
      {
        hxc_l_tmp_enum_switch_result_n1 = 0;
        break;
      }
    case hxc_Option_Some_ha8dd5a59e40a:
      {
        if (hxc_l_value_hffb395be3233.hxc_tag != hxc_Option_Some_ha8dd5a59e40a)
        {
          abort();
        }
        struct hxc_Rule hxc_l_tmp_enum_payload_project_n0 = hxc_l_value_hffb395be3233.hxc_payload.hxc_Some.hxc_value;
        hxc_l_value_hcf71bd05628a = hxc_l_tmp_enum_payload_project_n0;
        if (hxc_record_9f230b68_retain(&hxc_l_value_hcf71bd05628a) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_rule = hxc_l_value_hcf71bd05628a;
        if (hxc_record_9f230b68_retain(&hxc_l_rule) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_tmp_call_result_n3 = hxc_EnumFixture_ruleValue(hxc_l_rule);
        hxc_l_tmp_enum_switch_result_n1 = hxc_l_tmp_call_result_n3;
        hxc_record_9f230b68_destroy(&hxc_l_rule);
        hxc_record_9f230b68_destroy(&hxc_l_value_hcf71bd05628a);
        break;
      }
    default:
      {
        abort();
      }
  }
  return hxc_l_tmp_enum_switch_result_n1;
}

int32_t hxc_EnumFixture_pairedIdentityValue(enum hxc_IdentityKind hxc_l_kind, struct hxc_IdentityValue hxc_l_value_hc42edaab0080)
{
  int32_t hxc_l_tmp_enum_switch_result_n2 = 0;
  switch (hxc_l_kind) {
    case hxc_IdentityKind_FirstIdentity:
      {
        int32_t hxc_l_tmp_conditional_result_n3 = 0;
        if (hxc_l_value_hc42edaab0080.hxc_tag == hxc_IdentityValue_FirstValue)
        {
          if (hxc_l_value_hc42edaab0080.hxc_tag != hxc_IdentityValue_FirstValue)
          {
            abort();
          }
          int32_t hxc_l_tmp_enum_payload_project_n0 = hxc_l_value_hc42edaab0080.hxc_payload.hxc_FirstValue.hxc_value;
          int32_t hxc_l_value_h16eaf8117e85 = hxc_l_tmp_enum_payload_project_n0;
          int32_t hxc_l_item_ha6e924ef4683 = hxc_l_value_h16eaf8117e85;
          hxc_l_tmp_conditional_result_n3 = hxc_l_item_ha6e924ef4683;
        }
        else
        {
          hxc_l_tmp_conditional_result_n3 = -1;
        }
        hxc_l_tmp_enum_switch_result_n2 = hxc_l_tmp_conditional_result_n3;
        break;
      }
    case hxc_IdentityKind_SecondIdentity:
      {
        int32_t hxc_l_tmp_conditional_result_n6 = 0;
        if (hxc_l_value_hc42edaab0080.hxc_tag == hxc_IdentityValue_SecondValue)
        {
          if (hxc_l_value_hc42edaab0080.hxc_tag != hxc_IdentityValue_SecondValue)
          {
            abort();
          }
          int32_t hxc_l_tmp_enum_payload_project_n4 = hxc_l_value_hc42edaab0080.hxc_payload.hxc_SecondValue.hxc_value;
          int32_t hxc_l_value_h5ba4cc9b0b76 = hxc_l_tmp_enum_payload_project_n4;
          int32_t hxc_l_item_hd36f183c49b6 = hxc_l_value_h5ba4cc9b0b76;
          hxc_l_tmp_conditional_result_n6 = hxc_l_item_hd36f183c49b6;
        }
        else
        {
          hxc_l_tmp_conditional_result_n6 = -1;
        }
        hxc_l_tmp_enum_switch_result_n2 = hxc_l_tmp_conditional_result_n6;
        break;
      }
    default:
      {
        abort();
      }
  }
  return hxc_l_tmp_enum_switch_result_n2;
}

struct hxc_RecursiveActionPlan hxc_EnumFixture_recursiveActionPlan(void)
{
  const void *volatile hxc_l_gc_roots[7] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 7, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[0] = (struct hxc_RecursiveAction){ .hxc_tag = hxc_RecursiveAction_LeafAction, .hxc_payload.hxc_LeafAction.hxc_value = 17 }.hxc_tag == hxc_RecursiveAction_ChooseAction ? (const void *)(struct hxc_RecursiveAction){ .hxc_tag = hxc_RecursiveAction_LeafAction, .hxc_payload.hxc_LeafAction.hxc_value = 17 }.hxc_payload.hxc_ChooseAction.hxc_choices : NULL;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n1 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_array_eaf5e746_descriptor, (void **)&hxc_l_tmp_array_create_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_init_in_place(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_RecursiveAction), _Alignof(struct hxc_RecursiveAction), NULL, NULL, NULL, NULL }, hxc_l_tmp_array_create_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n1->value, &(struct hxc_RecursiveAction){ .hxc_tag = hxc_RecursiveAction_LeafAction, .hxc_payload.hxc_LeafAction.hxc_value = 17 }) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_array_create_result_n1;
  hxc_l_gc_roots[2] = (const void *)(struct hxc_RecursiveActionChoice){ .hxc_actions = hxc_l_tmp_array_create_result_n1, .hxc_weight = 1 }.hxc_actions;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n3 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_array_e4791f3e_descriptor, (void **)&hxc_l_tmp_array_create_result_n3) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_init_in_place(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_RecursiveActionChoice), _Alignof(struct hxc_RecursiveActionChoice), NULL, NULL, NULL, NULL }, hxc_l_tmp_array_create_result_n3) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n3->value, &(struct hxc_RecursiveActionChoice){ .hxc_actions = hxc_l_tmp_array_create_result_n1, .hxc_weight = 1 }) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_array_create_result_n3;
  hxc_l_gc_roots[4] = (struct hxc_RecursiveAction){ .hxc_tag = hxc_RecursiveAction_ChooseAction, .hxc_payload.hxc_ChooseAction.hxc_choices = hxc_l_tmp_array_create_result_n3 }.hxc_tag == hxc_RecursiveAction_ChooseAction ? (const void *)(struct hxc_RecursiveAction){ .hxc_tag = hxc_RecursiveAction_ChooseAction, .hxc_payload.hxc_ChooseAction.hxc_choices = hxc_l_tmp_array_create_result_n3 }.hxc_payload.hxc_ChooseAction.hxc_choices : NULL;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n5 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_array_eaf5e746_descriptor, (void **)&hxc_l_tmp_array_create_result_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_init_in_place(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_RecursiveAction), _Alignof(struct hxc_RecursiveAction), NULL, NULL, NULL, NULL }, hxc_l_tmp_array_create_result_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n5->value, &(struct hxc_RecursiveAction){ .hxc_tag = hxc_RecursiveAction_ChooseAction, .hxc_payload.hxc_ChooseAction.hxc_choices = hxc_l_tmp_array_create_result_n3 }) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_array_create_result_n5;
  hxc_l_gc_roots[6] = (const void *)(struct hxc_RecursiveActionPlan){ .hxc_actions = hxc_l_tmp_array_create_result_n5 }.hxc_actions;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_RecursiveActionPlan){ .hxc_actions = hxc_l_tmp_array_create_result_n5 };
}

int32_t hxc_EnumFixture_recursiveActionPlanValue(struct hxc_RecursiveActionPlan hxc_l_plan)
{
  const void *volatile hxc_l_gc_roots[16] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_plan.hxc_actions;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 16, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[1] = (const void *)hxc_l_plan.hxc_actions;
  struct hxc_RecursiveAction hxc_l_tmp_array_get_result_n1;
  if (hxc_array_ref_get_copy(hxc_l_plan.hxc_actions, (size_t)0, &hxc_l_tmp_array_get_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[2] = hxc_l_tmp_array_get_result_n1.hxc_tag == hxc_RecursiveAction_ChooseAction ? (const void *)hxc_l_tmp_array_get_result_n1.hxc_payload.hxc_ChooseAction.hxc_choices : NULL;
  struct hxc_RecursiveAction hxc_l_symbol = hxc_l_tmp_array_get_result_n1;
  struct hxc_RecursiveAction hxc_l_tmp_load_result_n2 = hxc_l_symbol;
  hxc_l_gc_roots[3] = hxc_l_tmp_load_result_n2.hxc_tag == hxc_RecursiveAction_ChooseAction ? (const void *)hxc_l_tmp_load_result_n2.hxc_payload.hxc_ChooseAction.hxc_choices : NULL;
  int32_t hxc_l_tmp_conditional_result_n2 = 0;
  if (hxc_l_tmp_load_result_n2.hxc_tag == hxc_RecursiveAction_ChooseAction)
  {
    hxc_l_gc_roots[4] = hxc_l_symbol.hxc_tag == hxc_RecursiveAction_ChooseAction ? (const void *)hxc_l_symbol.hxc_payload.hxc_ChooseAction.hxc_choices : NULL;
    if (hxc_l_symbol.hxc_tag != hxc_RecursiveAction_ChooseAction)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_enum_payload_project_n4 = hxc_l_symbol.hxc_payload.hxc_ChooseAction.hxc_choices;
    hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_enum_payload_project_n4;
    struct hxc_array_ref *hxc_l_choices = hxc_l_tmp_enum_payload_project_n4;
    hxc_l_gc_roots[6] = (const void *)hxc_l_choices;
    int32_t hxc_l_tmp_array_length_result_n6;
    if (hxc_array_ref_length(hxc_l_choices, &hxc_l_tmp_array_length_result_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_tmp_conditional_result_n4 = 0;
    if (hxc_l_tmp_array_length_result_n6 == 1)
    {
      hxc_l_gc_roots[7] = (const void *)hxc_l_choices;
      struct hxc_RecursiveActionChoice hxc_l_tmp_array_get_result_n8;
      if (hxc_array_ref_get_copy(hxc_l_choices, (size_t)0, &hxc_l_tmp_array_get_result_n8) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_gc_roots[8] = (const void *)hxc_l_tmp_array_get_result_n8.hxc_actions;
      struct hxc_RecursiveActionChoice hxc_l_a0_h20427e845f84 = hxc_l_tmp_array_get_result_n8;
      struct hxc_RecursiveActionChoice hxc_l_tmp_load_result_n9 = hxc_l_a0_h20427e845f84;
      hxc_l_gc_roots[9] = (const void *)hxc_l_tmp_load_result_n9.hxc_actions;
      hxc_l_gc_roots[10] = (const void *)hxc_l_a0_h20427e845f84.hxc_actions;
      struct hxc_array_ref *hxc_l_actions = hxc_l_a0_h20427e845f84.hxc_actions;
      int32_t hxc_l_weight = hxc_l_a0_h20427e845f84.hxc_weight;
      hxc_l_gc_roots[11] = (const void *)hxc_l_actions;
      int32_t hxc_l_tmp_array_length_result_n13;
      if (hxc_array_ref_length(hxc_l_actions, &hxc_l_tmp_array_length_result_n13) != HXC_STATUS_OK)
      {
        abort();
      }
      int32_t hxc_l_tmp_conditional_result_n8 = 0;
      if (hxc_l_tmp_array_length_result_n13 == 1)
      {
        hxc_l_gc_roots[12] = (const void *)hxc_l_actions;
        struct hxc_RecursiveAction hxc_l_tmp_array_get_result_n15;
        if (hxc_array_ref_get_copy(hxc_l_actions, (size_t)0, &hxc_l_tmp_array_get_result_n15) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_gc_roots[13] = hxc_l_tmp_array_get_result_n15.hxc_tag == hxc_RecursiveAction_ChooseAction ? (const void *)hxc_l_tmp_array_get_result_n15.hxc_payload.hxc_ChooseAction.hxc_choices : NULL;
        struct hxc_RecursiveAction hxc_l_a0_h761ba892ea54 = hxc_l_tmp_array_get_result_n15;
        struct hxc_RecursiveAction hxc_l_tmp_load_result_n16 = hxc_l_a0_h761ba892ea54;
        hxc_l_gc_roots[14] = hxc_l_tmp_load_result_n16.hxc_tag == hxc_RecursiveAction_ChooseAction ? (const void *)hxc_l_tmp_load_result_n16.hxc_payload.hxc_ChooseAction.hxc_choices : NULL;
        int32_t hxc_l_tmp_conditional_result_n10 = 0;
        if (hxc_l_tmp_load_result_n16.hxc_tag == hxc_RecursiveAction_LeafAction)
        {
          hxc_l_gc_roots[15] = hxc_l_a0_h761ba892ea54.hxc_tag == hxc_RecursiveAction_ChooseAction ? (const void *)hxc_l_a0_h761ba892ea54.hxc_payload.hxc_ChooseAction.hxc_choices : NULL;
          if (hxc_l_a0_h761ba892ea54.hxc_tag != hxc_RecursiveAction_LeafAction)
          {
            abort();
          }
          int32_t hxc_l_tmp_enum_payload_project_n18 = hxc_l_a0_h761ba892ea54.hxc_payload.hxc_LeafAction.hxc_value;
          int32_t hxc_l_value_h8011a7a07e37 = hxc_l_tmp_enum_payload_project_n18;
          int32_t hxc_l_tmp_load_result_n19 = hxc_l_weight;
          int32_t hxc_l_tmp_conditional_result_n12 = 0;
          if (hxc_l_tmp_load_result_n19 == 1)
          {
            int32_t hxc_l_value_h875e00836e25 = hxc_l_value_h8011a7a07e37;
            hxc_l_tmp_conditional_result_n12 = hxc_l_value_h875e00836e25;
          }
          else
          {
            hxc_l_tmp_conditional_result_n12 = 0;
          }
          hxc_l_tmp_conditional_result_n10 = hxc_l_tmp_conditional_result_n12;
        }
        else
        {
          hxc_l_tmp_conditional_result_n10 = 0;
        }
        hxc_l_tmp_conditional_result_n8 = hxc_l_tmp_conditional_result_n10;
      }
      else
      {
        hxc_l_tmp_conditional_result_n8 = 0;
      }
      hxc_l_tmp_conditional_result_n4 = hxc_l_tmp_conditional_result_n8;
    }
    else
    {
      hxc_l_tmp_conditional_result_n4 = 0;
    }
    hxc_l_tmp_conditional_result_n2 = hxc_l_tmp_conditional_result_n4;
  }
  else
  {
    hxc_l_tmp_conditional_result_n2 = 0;
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_conditional_result_n2;
}

int32_t hxc_EnumFixture_recursiveLocal(void)
{
  struct hxc_Chain hxc_l_next_h07fb61766894 = { 0 };
  struct hxc_Chain hxc_l_next_he440e694a9fe = { 0 };
  struct hxc_Chain hxc_l_tail = (struct hxc_Chain){ .hxc_tag = hxc_Chain_End, .hxc_payload.hxc_End.hxc_value = 2 };
  struct hxc_Chain hxc_l_tmp_enum_payload_1_owner_n2 = hxc_l_tail;
  if (hxc_enum_39285fe9_retain(&hxc_l_tmp_enum_payload_1_owner_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_Chain hxc_l_tmp_enum_payload_1_owned_load_result_n2 = hxc_l_tmp_enum_payload_1_owner_n2;
  struct hxc_Chain *hxc_l_tmp_enum_recursive_payload_owner_n3 = NULL;
  hxc_allocator hxc_l_tmp_enum_recursive_payload_owner_n3_allocator = hxc_default_allocator();
  if (hxc_alloc(&hxc_l_tmp_enum_recursive_payload_owner_n3_allocator, sizeof(struct hxc_Chain), _Alignof(struct hxc_Chain), (void **)&hxc_l_tmp_enum_recursive_payload_owner_n3) != HXC_STATUS_OK)
  {
    abort();
  }
  *hxc_l_tmp_enum_recursive_payload_owner_n3 = hxc_l_tmp_enum_payload_1_owned_load_result_n2;
  struct hxc_Chain hxc_l_head = (struct hxc_Chain){ .hxc_tag = hxc_Chain_Link, .hxc_payload.hxc_Link.hxc_value = 1, .hxc_payload.hxc_Link.hxc_next = hxc_l_tmp_enum_recursive_payload_owner_n3 };
  struct hxc_Chain hxc_l_tmp_load_result_n5 = hxc_l_head;
  int32_t hxc_l_tmp_enum_switch_result_n3 = 0;
  switch (hxc_l_tmp_load_result_n5.hxc_tag) {
    case hxc_Chain_End:
      {
        if (hxc_l_head.hxc_tag != hxc_Chain_End)
        {
          abort();
        }
        int32_t hxc_l_tmp_enum_payload_project_n7 = hxc_l_head.hxc_payload.hxc_End.hxc_value;
        int32_t hxc_l_value_h34f86739fe15 = hxc_l_tmp_enum_payload_project_n7;
        int32_t hxc_l_value_hc78beb4810c4 = hxc_l_value_h34f86739fe15;
        hxc_l_tmp_enum_switch_result_n3 = hxc_l_value_hc78beb4810c4;
        break;
      }
    case hxc_Chain_Link:
      {
        if (hxc_l_head.hxc_tag != hxc_Chain_Link)
        {
          abort();
        }
        int32_t hxc_l_tmp_enum_payload_project_n11 = hxc_l_head.hxc_payload.hxc_Link.hxc_value;
        int32_t hxc_l_value_h279cf5a91399 = hxc_l_tmp_enum_payload_project_n11;
        if (hxc_l_head.hxc_tag != hxc_Chain_Link)
        {
          abort();
        }
        struct hxc_Chain *hxc_l_tmp_enum_payload_project_n13 = hxc_l_head.hxc_payload.hxc_Link.hxc_next;
        struct hxc_Chain hxc_l_tmp_enum_recursive_payload_load_result_n14 = *hxc_l_tmp_enum_payload_project_n13;
        hxc_l_next_h07fb61766894 = hxc_l_tmp_enum_recursive_payload_load_result_n14;
        if (hxc_enum_39285fe9_retain(&hxc_l_next_h07fb61766894) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_l_value_hba17dd6c799c = hxc_l_value_h279cf5a91399;
        hxc_l_next_he440e694a9fe = hxc_l_next_h07fb61766894;
        if (hxc_enum_39285fe9_retain(&hxc_l_next_he440e694a9fe) != HXC_STATUS_OK)
        {
          abort();
        }
        struct hxc_Chain hxc_l_tmp_load_result_n17 = hxc_l_next_he440e694a9fe;
        int32_t hxc_l_tmp_enum_switch_result_n10 = 0;
        switch (hxc_l_tmp_load_result_n17.hxc_tag) {
          case hxc_Chain_End:
            {
              if (hxc_l_next_he440e694a9fe.hxc_tag != hxc_Chain_End)
              {
                abort();
              }
              int32_t hxc_l_tmp_enum_payload_project_n19 = hxc_l_next_he440e694a9fe.hxc_payload.hxc_End.hxc_value;
              int32_t hxc_l_value_h443e3a509706 = hxc_l_tmp_enum_payload_project_n19;
              int32_t hxc_l_last = hxc_l_value_h443e3a509706;
              int32_t hxc_l_tmp_load_result_n21 = hxc_l_value_hba17dd6c799c;
              hxc_l_tmp_enum_switch_result_n10 = hxc_i32_add_wrapping(hxc_l_tmp_load_result_n21, hxc_l_last);
              break;
            }
          case hxc_Chain_Link:
            {
              hxc_l_tmp_enum_switch_result_n10 = 0;
              break;
            }
          default:
            {
              abort();
            }
        }
        hxc_l_tmp_enum_switch_result_n3 = hxc_l_tmp_enum_switch_result_n10;
        hxc_enum_39285fe9_destroy(&hxc_l_next_he440e694a9fe);
        hxc_enum_39285fe9_destroy(&hxc_l_next_h07fb61766894);
        break;
      }
    default:
      {
        abort();
      }
  }
  int32_t hxc_l_tmp_enum_switch_result_load_result_n24 = hxc_l_tmp_enum_switch_result_n3;
  hxc_enum_39285fe9_destroy(&hxc_l_head);
  hxc_enum_39285fe9_destroy(&hxc_l_tail);
  return hxc_l_tmp_enum_switch_result_load_result_n24;
}

int32_t hxc_EnumFixture_ruleLiteralValue(struct hxc_Chain hxc_l_chain, struct hxc_Choices hxc_l_choices, struct hxc_array_ref *hxc_l_actions, struct hxc_Rule hxc_l_borrowed)
{
  struct hxc_Chain hxc_l_tmp_record_field_chain_owner_n5 = hxc_l_chain;
  if (hxc_enum_39285fe9_retain(&hxc_l_tmp_record_field_chain_owner_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_Chain hxc_l_tmp_record_field_chain_owned_load_result_n0 = hxc_l_tmp_record_field_chain_owner_n5;
  struct hxc_Choices hxc_l_tmp_record_field_choices_owner_n6 = hxc_l_choices;
  if (hxc_enum_d215f611_retain(&hxc_l_tmp_record_field_choices_owner_n6) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_Choices hxc_l_tmp_record_field_choices_owned_load_result_n1 = hxc_l_tmp_record_field_choices_owner_n6;
  struct hxc_array_ref *hxc_l_tmp_record_field_actions_owner_n7 = hxc_l_actions;
  if (hxc_array_ref_retain(hxc_l_tmp_record_field_actions_owner_n7) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_Rule hxc_l_tmp_array_literal_element_0_owner_n8 = (struct hxc_Rule){ .hxc_actions = hxc_l_tmp_record_field_actions_owner_n7, .hxc_chain = hxc_l_tmp_record_field_chain_owned_load_result_n0, .hxc_choices = hxc_l_tmp_record_field_choices_owned_load_result_n1 };
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n5 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_Rule), _Alignof(struct hxc_Rule), NULL, hxc_array_400559e4_element_copy, hxc_array_400559e4_element_assign, hxc_array_400559e4_element_destroy }, &hxc_l_tmp_array_create_result_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n5->value, &hxc_l_tmp_array_literal_element_0_owner_n8) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n5->value, &hxc_l_borrowed) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_rules = hxc_l_tmp_array_create_result_n5;
  int32_t hxc_l_tmp_array_length_result_n7;
  if (hxc_array_ref_length(hxc_l_rules, &hxc_l_tmp_array_length_result_n7) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_tmp_call_result_n8 = hxc_EnumFixture_ruleValue(hxc_l_borrowed);
  if (hxc_array_ref_release(hxc_l_rules) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_record_9f230b68_destroy(&hxc_l_tmp_array_literal_element_0_owner_n8);
  return hxc_i32_add_wrapping(hxc_l_tmp_array_length_result_n7, hxc_l_tmp_call_result_n8);
}

int32_t hxc_EnumFixture_ruleValue(struct hxc_Rule hxc_l_value)
{
  int32_t hxc_l_tmp_call_result_n1 = hxc_EnumFixture_chainValue(hxc_l_value.hxc_chain);
  int32_t hxc_l_tmp_call_result_n3 = hxc_EnumFixture_choiceValue(hxc_l_value.hxc_choices);
  int32_t hxc_l_tmp_array_get_result_n5;
  if (hxc_array_ref_get_copy(hxc_l_value.hxc_actions, (size_t)0, &hxc_l_tmp_array_get_result_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_l_tmp_call_result_n1, hxc_l_tmp_call_result_n3), hxc_l_tmp_array_get_result_n5);
}

struct hxc_StrictCarrierHolder hxc_EnumFixture_strictCarrierHolder(enum hxc_IdentityKind hxc_l_kind)
{
  struct hxc_StrictCarrier hxc_l_tmp_conditional_result_n2 = { 0 };
  if (hxc_l_kind == hxc_IdentityKind_FirstIdentity)
  {
    hxc_l_tmp_conditional_result_n2 = (struct hxc_StrictCarrier){ .hxc_tag = hxc_StrictCarrier_StrictEmpty };
  }
  else
  {
    hxc_l_tmp_conditional_result_n2 = (struct hxc_StrictCarrier){ .hxc_tag = hxc_StrictCarrier_StrictSmall, .hxc_payload.hxc_StrictSmall.hxc_value = 23 };
  }
  struct hxc_StrictCarrier hxc_l_selected = hxc_l_tmp_conditional_result_n2;
  return (struct hxc_StrictCarrierHolder){ .hxc_value = hxc_l_selected };
}

int32_t hxc_EnumFixture_strictCarrierValue(struct hxc_StrictCarrierHolder hxc_l_holder)
{
  struct hxc_StrictCarrier hxc_l_symbol = hxc_l_holder.hxc_value;
  struct hxc_StrictCarrier hxc_l_tmp_load_result_n1 = hxc_l_symbol;
  int32_t hxc_l_tmp_enum_switch_result_n2 = 0;
  switch (hxc_l_tmp_load_result_n1.hxc_tag) {
    case hxc_StrictCarrier_StrictEmpty:
      {
        hxc_l_tmp_enum_switch_result_n2 = 0;
        break;
      }
    case hxc_StrictCarrier_StrictSmall:
      {
        if (hxc_l_symbol.hxc_tag != hxc_StrictCarrier_StrictSmall)
        {
          abort();
        }
        int32_t hxc_l_tmp_enum_payload_project_n3 = hxc_l_symbol.hxc_payload.hxc_StrictSmall.hxc_value;
        int32_t hxc_l_value_h435b63311112 = hxc_l_tmp_enum_payload_project_n3;
        int32_t hxc_l_value_h91f5367e67be = hxc_l_value_h435b63311112;
        hxc_l_tmp_enum_switch_result_n2 = hxc_l_value_h91f5367e67be;
        break;
      }
    case hxc_StrictCarrier_StrictWide:
      {
        if (hxc_l_symbol.hxc_tag != hxc_StrictCarrier_StrictWide)
        {
          abort();
        }
        int32_t hxc_l_tmp_enum_payload_project_n7 = hxc_l_symbol.hxc_payload.hxc_StrictWide.hxc_first;
        int32_t hxc_l_first_h32a30c54819a = hxc_l_tmp_enum_payload_project_n7;
        if (hxc_l_symbol.hxc_tag != hxc_StrictCarrier_StrictWide)
        {
          abort();
        }
        int32_t hxc_l_tmp_enum_payload_project_n9 = hxc_l_symbol.hxc_payload.hxc_StrictWide.hxc_second;
        int32_t hxc_l_second_he6a110b34fec = hxc_l_tmp_enum_payload_project_n9;
        if (hxc_l_symbol.hxc_tag != hxc_StrictCarrier_StrictWide)
        {
          abort();
        }
        int32_t hxc_l_tmp_enum_payload_project_n11 = hxc_l_symbol.hxc_payload.hxc_StrictWide.hxc_third;
        int32_t hxc_l_third_he398dc69f275 = hxc_l_tmp_enum_payload_project_n11;
        if (hxc_l_symbol.hxc_tag != hxc_StrictCarrier_StrictWide)
        {
          abort();
        }
        int32_t hxc_l_tmp_enum_payload_project_n13 = hxc_l_symbol.hxc_payload.hxc_StrictWide.hxc_fourth;
        int32_t hxc_l_fourth_h78c905e0a7f6 = hxc_l_tmp_enum_payload_project_n13;
        int32_t hxc_l_first_he9ed2f72d824 = hxc_l_first_h32a30c54819a;
        int32_t hxc_l_second_h80e6c9476617 = hxc_l_second_he6a110b34fec;
        int32_t hxc_l_third_h9c8a87c6953a = hxc_l_third_he398dc69f275;
        int32_t hxc_l_fourth_h64a5cdc45ba4 = hxc_l_fourth_h78c905e0a7f6;
        int32_t hxc_l_tmp_load_result_n18 = hxc_l_first_he9ed2f72d824;
        int32_t hxc_l_tmp_load_result_n19 = hxc_l_second_h80e6c9476617;
        int32_t hxc_l_tmp_load_result_n20 = hxc_l_third_h9c8a87c6953a;
        hxc_l_tmp_enum_switch_result_n2 = hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_l_tmp_load_result_n18, hxc_l_tmp_load_result_n19), hxc_l_tmp_load_result_n20), hxc_l_fourth_h64a5cdc45ba4);
        break;
      }
    default:
      {
        abort();
      }
  }
  return hxc_l_tmp_enum_switch_result_n2;
}

int32_t hxc_EnumFixture_tailValue(struct hxc_Chain hxc_l_value_h43dc2cb9ec11)
{
  int32_t hxc_l_tmp_enum_switch_result_n1 = 0;
  switch (hxc_l_value_h43dc2cb9ec11.hxc_tag) {
    case hxc_Chain_End:
      {
        if (hxc_l_value_h43dc2cb9ec11.hxc_tag != hxc_Chain_End)
        {
          abort();
        }
        int32_t hxc_l_tmp_enum_payload_project_n0 = hxc_l_value_h43dc2cb9ec11.hxc_payload.hxc_End.hxc_value;
        int32_t hxc_l_value_hce7cb8e32020 = hxc_l_tmp_enum_payload_project_n0;
        int32_t hxc_l_item = hxc_l_value_hce7cb8e32020;
        hxc_l_tmp_enum_switch_result_n1 = hxc_l_item;
        break;
      }
    case hxc_Chain_Link:
      {
        hxc_l_tmp_enum_switch_result_n1 = 0;
        break;
      }
    default:
      {
        abort();
      }
  }
  return hxc_l_tmp_enum_switch_result_n1;
}

struct hxc_RuleEnvelope hxc_EnumFixture_wrapRule(struct hxc_Rule hxc_l_value)
{
  struct hxc_Rule hxc_l_tmp_enum_payload_0_owner_n1 = hxc_l_value;
  if (hxc_record_9f230b68_retain(&hxc_l_tmp_enum_payload_0_owner_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_RuleEnvelope){ .hxc_tag = hxc_RuleEnvelope_WrappedRule, .hxc_payload.hxc_WrappedRule.hxc_rule = hxc_l_tmp_enum_payload_0_owner_n1 };
}

struct hxc_Option_h95f1c4a28dac hxc_Option_i32_Some_synchronous_callback_adapter(void *hxc_l_context, int32_t hxc_l_value)
{
  (void)hxc_l_context;
  return (struct hxc_Option_h95f1c4a28dac){ .hxc_tag = hxc_Option_Some_ha9454146ff01, .hxc_payload.hxc_Some.hxc_value = hxc_l_value };
}

int main(void)
{
  if (hxc_gc_init(&(struct hxc_gc_config){ hxc_default_allocator(), 1048576U, NULL, NULL }, &hxc_program_gc) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_thread_register(&hxc_program_gc, &hxc_program_gc_thread) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_EnumFixture_main();
  if (hxc_gc_thread_unregister(&hxc_program_gc_thread) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_dispose(&hxc_program_gc) != HXC_STATUS_OK)
  {
    abort();
  }
  return 0;
}
