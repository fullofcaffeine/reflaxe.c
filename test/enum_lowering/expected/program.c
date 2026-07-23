#include "hxc/program.h"

_Static_assert(offsetof(struct hxc_Rule, hxc_actions) == 0, "closed record hxc_Rule first field begins at offset zero");

_Static_assert(_Alignof(struct hxc_Rule) >= _Alignof(struct hxc_array_ref *), "closed record hxc_Rule alignment admits field 0");

_Static_assert(offsetof(struct hxc_Rule, hxc_chain) >= offsetof(struct hxc_Rule, hxc_actions) + sizeof(struct hxc_array_ref *), "closed record hxc_Rule field 1 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_Rule) >= _Alignof(struct hxc_Chain), "closed record hxc_Rule alignment admits field 1");

_Static_assert(offsetof(struct hxc_Rule, hxc_choices) >= offsetof(struct hxc_Rule, hxc_chain) + sizeof(struct hxc_Chain), "closed record hxc_Rule field 2 follows the prior field without overlap");

_Static_assert(_Alignof(struct hxc_Rule) >= _Alignof(struct hxc_Choices), "closed record hxc_Rule alignment admits field 2");

_Static_assert(sizeof(struct hxc_Rule) >= offsetof(struct hxc_Rule, hxc_choices) + sizeof(struct hxc_Choices), "closed record hxc_Rule size contains its final field");

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

_Static_assert(hxc_Choices_NoChoices == 0, "enum hxc_Choices case NoChoices retains its Haxe discriminant");

_Static_assert(hxc_Choices_ChoiceValues == 1, "enum hxc_Choices case ChoiceValues retains its Haxe discriminant");

_Static_assert(offsetof(struct hxc_Choices, hxc_tag) == 0, "tagged enum hxc_Choices begins with its discriminant");

_Static_assert(offsetof(struct hxc_Choices, hxc_payload) >= sizeof(enum hxc_Choices_tag), "tagged enum hxc_Choices payload follows its discriminant");

_Static_assert(sizeof(struct hxc_Choices) >= offsetof(struct hxc_Choices, hxc_payload) + sizeof(union hxc_Choices_payload), "tagged enum hxc_Choices contains its payload union");

_Static_assert(offsetof(union hxc_Choices_payload, hxc_ChoiceValues) == 0, "tagged enum hxc_Choices case ChoiceValues begins at union offset zero");

_Static_assert(offsetof(struct hxc_Choices_ChoiceValues_payload, hxc_values) == 0, "tagged enum hxc_Choices case ChoiceValues first payload begins at zero");

_Static_assert(_Alignof(struct hxc_Choices_ChoiceValues_payload) >= _Alignof(struct hxc_array_ref *), "tagged enum hxc_Choices case ChoiceValues admits payload 0 alignment");

_Static_assert(hxc_RuleEnvelope_MissingRule == 0, "enum hxc_RuleEnvelope case MissingRule retains its Haxe discriminant");

_Static_assert(hxc_RuleEnvelope_WrappedRule == 1, "enum hxc_RuleEnvelope case WrappedRule retains its Haxe discriminant");

_Static_assert(offsetof(struct hxc_RuleEnvelope, hxc_tag) == 0, "tagged enum hxc_RuleEnvelope begins with its discriminant");

_Static_assert(offsetof(struct hxc_RuleEnvelope, hxc_payload) >= sizeof(enum hxc_RuleEnvelope_tag), "tagged enum hxc_RuleEnvelope payload follows its discriminant");

_Static_assert(sizeof(struct hxc_RuleEnvelope) >= offsetof(struct hxc_RuleEnvelope, hxc_payload) + sizeof(union hxc_RuleEnvelope_payload), "tagged enum hxc_RuleEnvelope contains its payload union");

_Static_assert(offsetof(union hxc_RuleEnvelope_payload, hxc_WrappedRule) == 0, "tagged enum hxc_RuleEnvelope case WrappedRule begins at union offset zero");

_Static_assert(offsetof(struct hxc_RuleEnvelope_WrappedRule_payload, hxc_rule) == 0, "tagged enum hxc_RuleEnvelope case WrappedRule first payload begins at zero");

_Static_assert(_Alignof(struct hxc_RuleEnvelope_WrappedRule_payload) >= _Alignof(struct hxc_Rule), "tagged enum hxc_RuleEnvelope case WrappedRule admits payload 0 alignment");

hxc_status hxc_record_9f230b68_retain(void *hxc_value)
{
  hxc_status hxc_operation_status;
  hxc_operation_status = hxc_array_ref_retain((*(struct hxc_Rule *)hxc_value).hxc_actions);
  if (hxc_operation_status != HXC_STATUS_OK)
  {
    return hxc_operation_status;
  }
  hxc_operation_status = hxc_enum_39285fe9_retain(&(*(struct hxc_Rule *)hxc_value).hxc_chain);
  if (hxc_operation_status != HXC_STATUS_OK)
  {
    (void)hxc_array_ref_release((*(struct hxc_Rule *)hxc_value).hxc_actions);
    return hxc_operation_status;
  }
  hxc_operation_status = hxc_enum_d215f611_retain(&(*(struct hxc_Rule *)hxc_value).hxc_choices);
  if (hxc_operation_status != HXC_STATUS_OK)
  {
    (void)hxc_enum_39285fe9_destroy(&(*(struct hxc_Rule *)hxc_value).hxc_chain);
    (void)hxc_array_ref_release((*(struct hxc_Rule *)hxc_value).hxc_actions);
    return hxc_operation_status;
  }
  return HXC_STATUS_OK;
}

void hxc_record_9f230b68_destroy(void *hxc_value)
{
  (void)hxc_enum_d215f611_destroy(&(*(struct hxc_Rule *)hxc_value).hxc_choices);
  (void)hxc_enum_39285fe9_destroy(&(*(struct hxc_Rule *)hxc_value).hxc_chain);
  (void)hxc_array_ref_release((*(struct hxc_Rule *)hxc_value).hxc_actions);
}

hxc_status hxc_enum_24936704_retain(void *hxc_value)
{
  hxc_status hxc_operation_status;
  switch ((*(struct hxc_Option_h2a07afaff02e *)hxc_value).hxc_tag) {
    case hxc_Option_None_hdcfb48028a4b:
      {
        break;
      }
    case hxc_Option_Some_ha8dd5a59e40a:
      {
        hxc_operation_status = hxc_record_9f230b68_retain(&(*(struct hxc_Option_h2a07afaff02e *)hxc_value).hxc_payload.hxc_Some.hxc_value);
        if (hxc_operation_status != HXC_STATUS_OK)
        {
          return hxc_operation_status;
        }
        break;
      }
  }
  return HXC_STATUS_OK;
}

void hxc_enum_24936704_destroy(void *hxc_value)
{
  switch ((*(struct hxc_Option_h2a07afaff02e *)hxc_value).hxc_tag) {
    case hxc_Option_None_hdcfb48028a4b:
      {
        break;
      }
    case hxc_Option_Some_ha8dd5a59e40a:
      {
        (void)hxc_record_9f230b68_destroy(&(*(struct hxc_Option_h2a07afaff02e *)hxc_value).hxc_payload.hxc_Some.hxc_value);
        break;
      }
  }
}

hxc_status hxc_enum_39285fe9_retain(void *hxc_value)
{
  hxc_status hxc_operation_status;
  switch ((*(struct hxc_Chain *)hxc_value).hxc_tag) {
    case hxc_Chain_End:
      {
        break;
      }
    case hxc_Chain_Link:
      {
        hxc_operation_status = hxc_enum_39285fe9_retain_recursive_clone(&(*(struct hxc_Chain *)hxc_value).hxc_payload.hxc_Link.hxc_next);
        if (hxc_operation_status != HXC_STATUS_OK)
        {
          return hxc_operation_status;
        }
        break;
      }
  }
  return HXC_STATUS_OK;
}

void hxc_enum_39285fe9_destroy(void *hxc_value)
{
  switch ((*(struct hxc_Chain *)hxc_value).hxc_tag) {
    case hxc_Chain_End:
      {
        break;
      }
    case hxc_Chain_Link:
      {
        (void)hxc_enum_39285fe9_destroy_recursive_destroy(&(*(struct hxc_Chain *)hxc_value).hxc_payload.hxc_Link.hxc_next);
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

hxc_status hxc_enum_d215f611_retain(void *hxc_value)
{
  hxc_status hxc_operation_status;
  switch ((*(struct hxc_Choices *)hxc_value).hxc_tag) {
    case hxc_Choices_NoChoices:
      {
        break;
      }
    case hxc_Choices_ChoiceValues:
      {
        hxc_operation_status = hxc_array_ref_retain((*(struct hxc_Choices *)hxc_value).hxc_payload.hxc_ChoiceValues.hxc_values);
        if (hxc_operation_status != HXC_STATUS_OK)
        {
          return hxc_operation_status;
        }
        break;
      }
  }
  return HXC_STATUS_OK;
}

void hxc_enum_d215f611_destroy(void *hxc_value)
{
  switch ((*(struct hxc_Choices *)hxc_value).hxc_tag) {
    case hxc_Choices_NoChoices:
      {
        break;
      }
    case hxc_Choices_ChoiceValues:
      {
        (void)hxc_array_ref_release((*(struct hxc_Choices *)hxc_value).hxc_payload.hxc_ChoiceValues.hxc_values);
        break;
      }
  }
}

hxc_status hxc_enum_ffce8027_retain(void *hxc_value)
{
  hxc_status hxc_operation_status;
  switch ((*(struct hxc_RuleEnvelope *)hxc_value).hxc_tag) {
    case hxc_RuleEnvelope_MissingRule:
      {
        break;
      }
    case hxc_RuleEnvelope_WrappedRule:
      {
        hxc_operation_status = hxc_record_9f230b68_retain(&(*(struct hxc_RuleEnvelope *)hxc_value).hxc_payload.hxc_WrappedRule.hxc_rule);
        if (hxc_operation_status != HXC_STATUS_OK)
        {
          return hxc_operation_status;
        }
        break;
      }
  }
  return HXC_STATUS_OK;
}

void hxc_enum_ffce8027_destroy(void *hxc_value)
{
  switch ((*(struct hxc_RuleEnvelope *)hxc_value).hxc_tag) {
    case hxc_RuleEnvelope_MissingRule:
      {
        break;
      }
    case hxc_RuleEnvelope_WrappedRule:
      {
        (void)hxc_record_9f230b68_destroy(&(*(struct hxc_RuleEnvelope *)hxc_value).hxc_payload.hxc_WrappedRule.hxc_rule);
        break;
      }
  }
}

hxc_status hxc_array_400559e4_element_copy(void *hxc_context, void *hxc_destination, const void *hxc_source)
{
  (void)hxc_context;
  hxc_status hxc_operation_status;
  *(struct hxc_Rule *)hxc_destination = *(const struct hxc_Rule *)hxc_source;
  hxc_operation_status = hxc_array_ref_retain((*(struct hxc_Rule *)hxc_destination).hxc_actions);
  if (hxc_operation_status != HXC_STATUS_OK)
  {
    return hxc_operation_status;
  }
  hxc_operation_status = hxc_enum_39285fe9_retain(&(*(struct hxc_Rule *)hxc_destination).hxc_chain);
  if (hxc_operation_status != HXC_STATUS_OK)
  {
    (void)hxc_array_ref_release((*(struct hxc_Rule *)hxc_destination).hxc_actions);
    return hxc_operation_status;
  }
  hxc_operation_status = hxc_enum_d215f611_retain(&(*(struct hxc_Rule *)hxc_destination).hxc_choices);
  if (hxc_operation_status != HXC_STATUS_OK)
  {
    (void)hxc_enum_39285fe9_destroy(&(*(struct hxc_Rule *)hxc_destination).hxc_chain);
    (void)hxc_array_ref_release((*(struct hxc_Rule *)hxc_destination).hxc_actions);
    return hxc_operation_status;
  }
  return HXC_STATUS_OK;
}

hxc_status hxc_array_400559e4_element_assign(void *hxc_context, void *hxc_destination, const void *hxc_source)
{
  (void)hxc_context;
  if (hxc_destination == hxc_source)
  {
    return HXC_STATUS_OK;
  }
  hxc_status hxc_operation_status;
  struct hxc_Rule hxc_array_400559e4_element_assign_replacement = *(const struct hxc_Rule *)hxc_source;
  hxc_operation_status = hxc_array_ref_retain(hxc_array_400559e4_element_assign_replacement.hxc_actions);
  if (hxc_operation_status != HXC_STATUS_OK)
  {
    return hxc_operation_status;
  }
  hxc_operation_status = hxc_enum_39285fe9_retain(&hxc_array_400559e4_element_assign_replacement.hxc_chain);
  if (hxc_operation_status != HXC_STATUS_OK)
  {
    (void)hxc_array_ref_release(hxc_array_400559e4_element_assign_replacement.hxc_actions);
    return hxc_operation_status;
  }
  hxc_operation_status = hxc_enum_d215f611_retain(&hxc_array_400559e4_element_assign_replacement.hxc_choices);
  if (hxc_operation_status != HXC_STATUS_OK)
  {
    (void)hxc_enum_39285fe9_destroy(&hxc_array_400559e4_element_assign_replacement.hxc_chain);
    (void)hxc_array_ref_release(hxc_array_400559e4_element_assign_replacement.hxc_actions);
    return hxc_operation_status;
  }
  (void)hxc_enum_d215f611_destroy(&(*(struct hxc_Rule *)hxc_destination).hxc_choices);
  (void)hxc_enum_39285fe9_destroy(&(*(struct hxc_Rule *)hxc_destination).hxc_chain);
  (void)hxc_array_ref_release((*(struct hxc_Rule *)hxc_destination).hxc_actions);
  *(struct hxc_Rule *)hxc_destination = hxc_array_400559e4_element_assign_replacement;
  return HXC_STATUS_OK;
}

void hxc_array_400559e4_element_destroy(void *hxc_context, void *hxc_element)
{
  (void)hxc_context;
  (void)hxc_enum_d215f611_destroy(&(*(struct hxc_Rule *)hxc_element).hxc_choices);
  (void)hxc_enum_39285fe9_destroy(&(*(struct hxc_Rule *)hxc_element).hxc_chain);
  (void)hxc_array_ref_release((*(struct hxc_Rule *)hxc_element).hxc_actions);
}

hxc_status hxc_array_84c38722_element_copy(void *hxc_context, void *hxc_destination, const void *hxc_source)
{
  (void)hxc_context;
  hxc_status hxc_operation_status;
  *(struct hxc_RuleEnvelope *)hxc_destination = *(const struct hxc_RuleEnvelope *)hxc_source;
  hxc_operation_status = hxc_enum_ffce8027_retain(&*(struct hxc_RuleEnvelope *)hxc_destination);
  if (hxc_operation_status != HXC_STATUS_OK)
  {
    return hxc_operation_status;
  }
  return HXC_STATUS_OK;
}

hxc_status hxc_array_84c38722_element_assign(void *hxc_context, void *hxc_destination, const void *hxc_source)
{
  (void)hxc_context;
  if (hxc_destination == hxc_source)
  {
    return HXC_STATUS_OK;
  }
  hxc_status hxc_operation_status;
  struct hxc_RuleEnvelope hxc_array_84c38722_element_assign_replacement = *(const struct hxc_RuleEnvelope *)hxc_source;
  hxc_operation_status = hxc_enum_ffce8027_retain(&hxc_array_84c38722_element_assign_replacement);
  if (hxc_operation_status != HXC_STATUS_OK)
  {
    return hxc_operation_status;
  }
  hxc_enum_ffce8027_destroy(&*(struct hxc_RuleEnvelope *)hxc_destination);
  *(struct hxc_RuleEnvelope *)hxc_destination = hxc_array_84c38722_element_assign_replacement;
  return HXC_STATUS_OK;
}

void hxc_array_84c38722_element_destroy(void *hxc_context, void *hxc_element)
{
  (void)hxc_context;
  hxc_enum_ffce8027_destroy(&*(struct hxc_RuleEnvelope *)hxc_element);
}

struct hxc_Option_h95f1c4a28dac hxc_EnumFixture_applyOption(int32_t hxc_value, struct hxc_Option_h95f1c4a28dac (*hxc_constructor)(int32_t))
{
  struct hxc_Option_h95f1c4a28dac hxc_tmp_indirect_call_result_n0 = hxc_constructor(hxc_value);
  return hxc_tmp_indirect_call_result_n0;
}

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

int32_t hxc_EnumFixture_chainValue(struct hxc_Chain hxc_value_h10794fed7059)
{
  int32_t hxc_tmp_enum_switch_result_n1 = 0;
  switch (hxc_value_h10794fed7059.hxc_tag) {
    case hxc_Chain_End:
      {
        if (hxc_value_h10794fed7059.hxc_tag != hxc_Chain_End)
        {
          abort();
        }
        int32_t hxc_tmp_enum_payload_project_n0 = hxc_value_h10794fed7059.hxc_payload.hxc_End.hxc_value;
        int32_t hxc_value_h81e07fd8e778 = hxc_tmp_enum_payload_project_n0;
        int32_t hxc_item_h867a45ae30df = hxc_value_h81e07fd8e778;
        hxc_tmp_enum_switch_result_n1 = hxc_item_h867a45ae30df;
        break;
      }
    case hxc_Chain_Link:
      {
        if (hxc_value_h10794fed7059.hxc_tag != hxc_Chain_Link)
        {
          abort();
        }
        int32_t hxc_tmp_enum_payload_project_n3 = hxc_value_h10794fed7059.hxc_payload.hxc_Link.hxc_value;
        int32_t hxc_value_hfb282a085de7 = hxc_tmp_enum_payload_project_n3;
        if (hxc_value_h10794fed7059.hxc_tag != hxc_Chain_Link)
        {
          abort();
        }
        struct hxc_Chain *hxc_tmp_enum_payload_project_n4 = hxc_value_h10794fed7059.hxc_payload.hxc_Link.hxc_next;
        struct hxc_Chain hxc_tmp_enum_recursive_payload_load_result_n5 = *hxc_tmp_enum_payload_project_n4;
        struct hxc_Chain hxc_next_hcce74b90370b = hxc_tmp_enum_recursive_payload_load_result_n5;
        if (hxc_enum_39285fe9_retain(&hxc_next_hcce74b90370b) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_item_h6d1bff9ec5ac = hxc_value_hfb282a085de7;
        struct hxc_Chain hxc_next_h24eca731d2de = hxc_next_hcce74b90370b;
        if (hxc_enum_39285fe9_retain(&hxc_next_h24eca731d2de) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_tmp_load_result_n8 = hxc_item_h6d1bff9ec5ac;
        int32_t hxc_tmp_call_result_n10 = hxc_EnumFixture_tailValue(hxc_next_h24eca731d2de);
        hxc_tmp_enum_switch_result_n1 = hxc_i32_add_wrapping(hxc_tmp_load_result_n8, hxc_tmp_call_result_n10);
        hxc_enum_39285fe9_destroy(&hxc_next_h24eca731d2de);
        hxc_enum_39285fe9_destroy(&hxc_next_hcce74b90370b);
        break;
      }
  }
  return hxc_tmp_enum_switch_result_n1;
}

int32_t hxc_EnumFixture_choiceValue(struct hxc_Choices hxc_value)
{
  int32_t hxc_tmp_enum_switch_result_n1 = 0;
  switch (hxc_value.hxc_tag) {
    case hxc_Choices_NoChoices:
      {
        hxc_tmp_enum_switch_result_n1 = 0;
        break;
      }
    case hxc_Choices_ChoiceValues:
      {
        if (hxc_value.hxc_tag != hxc_Choices_ChoiceValues)
        {
          abort();
        }
        struct hxc_array_ref *hxc_tmp_enum_payload_project_n0 = hxc_value.hxc_payload.hxc_ChoiceValues.hxc_values;
        struct hxc_array_ref *hxc_values = hxc_tmp_enum_payload_project_n0;
        struct hxc_array_ref *hxc_items = hxc_values;
        if (hxc_array_ref_retain(hxc_items) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_tmp_array_get_result_n3;
        if (hxc_array_ref_get_copy(hxc_items, (size_t)0, &hxc_tmp_array_get_result_n3) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_tmp_enum_switch_result_n1 = hxc_tmp_array_get_result_n3;
        if (hxc_array_ref_release(hxc_items) != HXC_STATUS_OK)
        {
          abort();
        }
        break;
      }
  }
  return hxc_tmp_enum_switch_result_n1;
}

int32_t hxc_EnumFixture_constructorValue(void)
{
  struct hxc_Option_h95f1c4a28dac hxc_tmp_call_result_n0 = hxc_EnumFixture_applyOption(9, hxc_Option_i32_Some_adapter);
  int32_t hxc_tmp_call_result_n1 = hxc_EnumFixture_optionValue(hxc_tmp_call_result_n0);
  return hxc_tmp_call_result_n1;
}

struct hxc_RuleEnvelope hxc_EnumFixture_copyEnvelope(struct hxc_RuleEnvelope hxc_value)
{
  struct hxc_RuleEnvelope hxc_tmp_returned_enum_owner_n1 = hxc_value;
  if (hxc_enum_ffce8027_retain(&hxc_tmp_returned_enum_owner_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_tmp_returned_enum_owner_n1;
}

struct hxc_Rule hxc_EnumFixture_copyRule(struct hxc_Rule hxc_value)
{
  struct hxc_Rule hxc_tmp_returned_record_owner_n1 = hxc_value;
  if (hxc_record_9f230b68_retain(&hxc_tmp_returned_record_owner_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_tmp_returned_record_owner_n1;
}

int32_t hxc_EnumFixture_envelopeValue(struct hxc_RuleEnvelope hxc_value)
{
  int32_t hxc_tmp_enum_switch_result_n1 = 0;
  switch (hxc_value.hxc_tag) {
    case hxc_RuleEnvelope_MissingRule:
      {
        hxc_tmp_enum_switch_result_n1 = 0;
        break;
      }
    case hxc_RuleEnvelope_WrappedRule:
      {
        if (hxc_value.hxc_tag != hxc_RuleEnvelope_WrappedRule)
        {
          abort();
        }
        struct hxc_Rule hxc_tmp_enum_payload_project_n0 = hxc_value.hxc_payload.hxc_WrappedRule.hxc_rule;
        struct hxc_Rule hxc_rule_h5227d8af703a = hxc_tmp_enum_payload_project_n0;
        if (hxc_record_9f230b68_retain(&hxc_rule_h5227d8af703a) != HXC_STATUS_OK)
        {
          abort();
        }
        struct hxc_Rule hxc_rule_hefedbce21b8f = hxc_rule_h5227d8af703a;
        if (hxc_record_9f230b68_retain(&hxc_rule_hefedbce21b8f) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_tmp_call_result_n3 = hxc_EnumFixture_ruleValue(hxc_rule_hefedbce21b8f);
        hxc_tmp_enum_switch_result_n1 = hxc_tmp_call_result_n3;
        hxc_record_9f230b68_destroy(&hxc_rule_hefedbce21b8f);
        hxc_record_9f230b68_destroy(&hxc_rule_h5227d8af703a);
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

enum hxc_Mode hxc_EnumFixture_identityMode(enum hxc_Mode hxc_value)
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
  struct hxc_array_ref *hxc_tmp_array_create_result_n5 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(int32_t), _Alignof(int32_t), &hxc_tmp_array_create_result_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_tmp_array_create_result_n5->value, &(int32_t){ 3 }) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_choices = hxc_tmp_array_create_result_n5;
  struct hxc_array_ref *hxc_tmp_array_create_result_n6 = NULL;
  if (hxc_array_ref_create_trivial(hxc_default_allocator(), sizeof(int32_t), _Alignof(int32_t), &hxc_tmp_array_create_result_n6) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_tmp_array_create_result_n6->value, &(int32_t){ 4 }) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_actions = hxc_tmp_array_create_result_n6;
  struct hxc_Chain *hxc_tmp_enum_recursive_payload_owner_n8 = NULL;
  hxc_allocator hxc_tmp_enum_recursive_payload_owner_n8_allocator = hxc_default_allocator();
  if (hxc_alloc(&hxc_tmp_enum_recursive_payload_owner_n8_allocator, sizeof(struct hxc_Chain), _Alignof(struct hxc_Chain), (void **)&hxc_tmp_enum_recursive_payload_owner_n8) != HXC_STATUS_OK)
  {
    abort();
  }
  *hxc_tmp_enum_recursive_payload_owner_n8 = (struct hxc_Chain){ .hxc_tag = hxc_Chain_End, .hxc_payload.hxc_End.hxc_value = 2 };
  struct hxc_Chain hxc_tmp_static_call_argument_0_owner_n7 = (struct hxc_Chain){ .hxc_tag = hxc_Chain_Link, .hxc_payload.hxc_Link.hxc_value = 1, .hxc_payload.hxc_Link.hxc_next = hxc_tmp_enum_recursive_payload_owner_n8 };
  struct hxc_Chain hxc_tmp_static_call_argument_0_borrow_result_n10 = hxc_tmp_static_call_argument_0_owner_n7;
  struct hxc_array_ref *hxc_tmp_enum_payload_0_owner_n8 = hxc_choices;
  if (hxc_array_ref_retain(hxc_tmp_enum_payload_0_owner_n8) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_Choices hxc_tmp_static_call_argument_1_owner_n9 = (struct hxc_Choices){ .hxc_tag = hxc_Choices_ChoiceValues, .hxc_payload.hxc_ChoiceValues.hxc_values = hxc_tmp_enum_payload_0_owner_n8 };
  struct hxc_Choices hxc_tmp_static_call_argument_1_borrow_result_n14 = hxc_tmp_static_call_argument_1_owner_n9;
  struct hxc_Rule hxc_tmp_call_result_n16 = hxc_EnumFixture_makeRule(hxc_tmp_static_call_argument_0_borrow_result_n10, hxc_tmp_static_call_argument_1_borrow_result_n14, hxc_actions);
  struct hxc_Rule hxc_rule = hxc_tmp_call_result_n16;
  struct hxc_Rule hxc_tmp_call_result_n18 = hxc_EnumFixture_copyRule(hxc_rule);
  struct hxc_Rule hxc_copiedRule = hxc_tmp_call_result_n18;
  struct hxc_RuleEnvelope hxc_tmp_call_result_n20 = hxc_EnumFixture_wrapRule(hxc_copiedRule);
  struct hxc_RuleEnvelope hxc_envelope = hxc_tmp_call_result_n20;
  struct hxc_RuleEnvelope hxc_tmp_call_result_n22 = hxc_EnumFixture_copyEnvelope(hxc_envelope);
  struct hxc_RuleEnvelope hxc_copiedEnvelope = hxc_tmp_call_result_n22;
  struct hxc_Rule hxc_tmp_enum_payload_0_owner_n14 = hxc_copiedRule;
  if (hxc_record_9f230b68_retain(&hxc_tmp_enum_payload_0_owner_n14) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_Option_h2a07afaff02e hxc_optionalRule = (struct hxc_Option_h2a07afaff02e){ .hxc_tag = hxc_Option_Some_ha8dd5a59e40a, .hxc_payload.hxc_Some.hxc_value = hxc_tmp_enum_payload_0_owner_n14 };
  struct hxc_array_ref *hxc_tmp_array_create_result_n26 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_Rule), _Alignof(struct hxc_Rule), NULL, hxc_array_400559e4_element_copy, hxc_array_400559e4_element_assign, hxc_array_400559e4_element_destroy }, &hxc_tmp_array_create_result_n26) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_rules = hxc_tmp_array_create_result_n26;
  struct hxc_array_ref *hxc_tmp_load_result_n27 = hxc_rules;
  int32_t hxc_tmp_array_push_result_n29;
  if (hxc_array_ref_push_copy(hxc_tmp_load_result_n27, &hxc_copiedRule, &hxc_tmp_array_push_result_n29) != HXC_STATUS_OK)
  {
    abort();
  }
  (void)hxc_tmp_array_push_result_n29;
  struct hxc_array_ref *hxc_tmp_array_create_result_n30 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_RuleEnvelope), _Alignof(struct hxc_RuleEnvelope), NULL, hxc_array_84c38722_element_copy, hxc_array_84c38722_element_assign, hxc_array_84c38722_element_destroy }, &hxc_tmp_array_create_result_n30) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_array_ref *hxc_envelopes = hxc_tmp_array_create_result_n30;
  struct hxc_array_ref *hxc_tmp_load_result_n31 = hxc_envelopes;
  int32_t hxc_tmp_array_push_result_n33;
  if (hxc_array_ref_push_copy(hxc_tmp_load_result_n31, &hxc_copiedEnvelope, &hxc_tmp_array_push_result_n33) != HXC_STATUS_OK)
  {
    abort();
  }
  (void)hxc_tmp_array_push_result_n33;
  while (1)
  {
    int32_t hxc_tmp_call_result_n35 = hxc_EnumFixture_modeValue(hxc_mode);
    bool hxc_tmp_short_circuit_result_n17 = hxc_tmp_call_result_n35 == 1;
    if (hxc_tmp_call_result_n35 == 1)
    {
      bool hxc_tmp_call_result_n36 = hxc_EnumFixture_modeEquality();
      hxc_tmp_short_circuit_result_n17 = hxc_tmp_call_result_n36;
    }
    bool hxc_tmp_short_circuit_load_result_n37 = hxc_tmp_short_circuit_result_n17;
    bool hxc_tmp_short_circuit_result_n18 = hxc_tmp_short_circuit_load_result_n37;
    if (hxc_tmp_short_circuit_load_result_n37)
    {
      int32_t hxc_tmp_call_result_n39 = hxc_EnumFixture_optionValue(hxc_present);
      hxc_tmp_short_circuit_result_n18 = hxc_tmp_call_result_n39 == 7;
    }
    bool hxc_tmp_short_circuit_load_result_n40 = hxc_tmp_short_circuit_result_n18;
    bool hxc_tmp_short_circuit_result_n19 = hxc_tmp_short_circuit_load_result_n40;
    if (hxc_tmp_short_circuit_load_result_n40)
    {
      int32_t hxc_tmp_call_result_n42 = hxc_EnumFixture_optionValue(hxc_absent);
      hxc_tmp_short_circuit_result_n19 = hxc_tmp_call_result_n42 == 0;
    }
    bool hxc_tmp_short_circuit_load_result_n43 = hxc_tmp_short_circuit_result_n19;
    bool hxc_tmp_short_circuit_result_n20 = hxc_tmp_short_circuit_load_result_n43;
    if (hxc_tmp_short_circuit_load_result_n43)
    {
      int32_t hxc_tmp_call_result_n44 = hxc_EnumFixture_constructorValue();
      hxc_tmp_short_circuit_result_n20 = hxc_tmp_call_result_n44 == 9;
    }
    bool hxc_tmp_short_circuit_load_result_n45 = hxc_tmp_short_circuit_result_n20;
    bool hxc_tmp_short_circuit_result_n21 = hxc_tmp_short_circuit_load_result_n45;
    if (hxc_tmp_short_circuit_load_result_n45)
    {
      int32_t hxc_tmp_call_result_n47 = hxc_EnumFixture_guardedValue(hxc_present);
      hxc_tmp_short_circuit_result_n21 = hxc_tmp_call_result_n47 == 7;
    }
    bool hxc_tmp_short_circuit_load_result_n48 = hxc_tmp_short_circuit_result_n21;
    bool hxc_tmp_short_circuit_result_n22 = hxc_tmp_short_circuit_load_result_n48;
    if (hxc_tmp_short_circuit_load_result_n48)
    {
      int32_t hxc_tmp_call_result_n50 = hxc_EnumFixture_boolOptionValue(hxc_truth);
      hxc_tmp_short_circuit_result_n22 = hxc_tmp_call_result_n50 == 1;
    }
    bool hxc_tmp_short_circuit_load_result_n51 = hxc_tmp_short_circuit_result_n22;
    bool hxc_tmp_short_circuit_result_n23 = hxc_tmp_short_circuit_load_result_n51;
    if (hxc_tmp_short_circuit_load_result_n51)
    {
      int32_t hxc_tmp_call_result_n52 = hxc_EnumFixture_recursiveLocal();
      hxc_tmp_short_circuit_result_n23 = hxc_tmp_call_result_n52 == 3;
    }
    bool hxc_tmp_short_circuit_load_result_n53 = hxc_tmp_short_circuit_result_n23;
    bool hxc_tmp_short_circuit_result_n24 = hxc_tmp_short_circuit_load_result_n53;
    if (hxc_tmp_short_circuit_load_result_n53)
    {
      int32_t hxc_tmp_call_result_n55 = hxc_EnumFixture_ruleValue(hxc_copiedRule);
      hxc_tmp_short_circuit_result_n24 = hxc_tmp_call_result_n55 == 10;
    }
    bool hxc_tmp_short_circuit_load_result_n56 = hxc_tmp_short_circuit_result_n24;
    bool hxc_tmp_short_circuit_result_n25 = hxc_tmp_short_circuit_load_result_n56;
    if (hxc_tmp_short_circuit_load_result_n56)
    {
      int32_t hxc_tmp_call_result_n58 = hxc_EnumFixture_envelopeValue(hxc_copiedEnvelope);
      hxc_tmp_short_circuit_result_n25 = hxc_tmp_call_result_n58 == 10;
    }
    bool hxc_tmp_short_circuit_load_result_n59 = hxc_tmp_short_circuit_result_n25;
    bool hxc_tmp_short_circuit_result_n26 = hxc_tmp_short_circuit_load_result_n59;
    if (hxc_tmp_short_circuit_load_result_n59)
    {
      int32_t hxc_tmp_call_result_n61 = hxc_EnumFixture_optionalRuleValue(hxc_optionalRule);
      hxc_tmp_short_circuit_result_n26 = hxc_tmp_call_result_n61 == 10;
    }
    bool hxc_tmp_short_circuit_load_result_n62 = hxc_tmp_short_circuit_result_n26;
    bool hxc_tmp_short_circuit_result_n27 = hxc_tmp_short_circuit_load_result_n62;
    if (hxc_tmp_short_circuit_load_result_n62)
    {
      int32_t hxc_tmp_array_length_result_n64;
      if (hxc_array_ref_length(hxc_envelopes, &hxc_tmp_array_length_result_n64) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_tmp_short_circuit_result_n27 = hxc_tmp_array_length_result_n64 == 1;
    }
    bool hxc_tmp_short_circuit_load_result_n65 = hxc_tmp_short_circuit_result_n27;
    bool hxc_tmp_short_circuit_result_n28 = hxc_tmp_short_circuit_load_result_n65;
    if (hxc_tmp_short_circuit_load_result_n65)
    {
      int32_t hxc_tmp_array_length_result_n67;
      if (hxc_array_ref_length(hxc_rules, &hxc_tmp_array_length_result_n67) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_tmp_short_circuit_result_n28 = hxc_tmp_array_length_result_n67 == 1;
    }
    if (!!hxc_tmp_short_circuit_result_n28)
    {
      break;
    }
  }
  if (hxc_array_ref_release(hxc_envelopes) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_rules) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_enum_24936704_destroy(&hxc_optionalRule);
  hxc_enum_ffce8027_destroy(&hxc_copiedEnvelope);
  hxc_enum_ffce8027_destroy(&hxc_envelope);
  hxc_record_9f230b68_destroy(&hxc_copiedRule);
  hxc_record_9f230b68_destroy(&hxc_rule);
  hxc_enum_d215f611_destroy(&hxc_tmp_static_call_argument_1_owner_n9);
  hxc_enum_39285fe9_destroy(&hxc_tmp_static_call_argument_0_owner_n7);
  if (hxc_array_ref_release(hxc_actions) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_choices) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

struct hxc_Rule hxc_EnumFixture_makeRule(struct hxc_Chain hxc_chain, struct hxc_Choices hxc_choices, struct hxc_array_ref *hxc_actions)
{
  struct hxc_Chain hxc_tmp_record_field_chain_owner_n3 = hxc_chain;
  if (hxc_enum_39285fe9_retain(&hxc_tmp_record_field_chain_owner_n3) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_Chain hxc_tmp_record_field_chain_owned_load_result_n0 = hxc_tmp_record_field_chain_owner_n3;
  struct hxc_Choices hxc_tmp_record_field_choices_owner_n4 = hxc_choices;
  if (hxc_enum_d215f611_retain(&hxc_tmp_record_field_choices_owner_n4) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_Choices hxc_tmp_record_field_choices_owned_load_result_n1 = hxc_tmp_record_field_choices_owner_n4;
  struct hxc_array_ref *hxc_tmp_record_field_actions_owner_n5 = hxc_actions;
  if (hxc_array_ref_retain(hxc_tmp_record_field_actions_owner_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_Rule){ .hxc_actions = hxc_tmp_record_field_actions_owner_n5, .hxc_chain = hxc_tmp_record_field_chain_owned_load_result_n0, .hxc_choices = hxc_tmp_record_field_choices_owned_load_result_n1 };
}

bool hxc_EnumFixture_modeEquality(void)
{
  enum hxc_Mode hxc_tmp_call_result_n1 = hxc_EnumFixture_identityMode(hxc_Mode_On);
  enum hxc_Mode hxc_tmp_call_result_n3 = hxc_EnumFixture_identityMode(hxc_Mode_On);
  bool hxc_same = hxc_tmp_call_result_n1 == hxc_tmp_call_result_n3;
  enum hxc_Mode hxc_tmp_call_result_n5 = hxc_EnumFixture_identityMode(hxc_Mode_Off);
  enum hxc_Mode hxc_tmp_call_result_n7 = hxc_EnumFixture_identityMode(hxc_Mode_On);
  bool hxc_different = hxc_tmp_call_result_n5 != hxc_tmp_call_result_n7;
  bool hxc_tmp_load_result_n8 = hxc_same;
  bool hxc_tmp_short_circuit_result_n2 = hxc_tmp_load_result_n8;
  if (hxc_tmp_load_result_n8)
  {
    hxc_tmp_short_circuit_result_n2 = hxc_different;
  }
  return hxc_tmp_short_circuit_result_n2;
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

int32_t hxc_EnumFixture_optionalRuleValue(struct hxc_Option_h2a07afaff02e hxc_value_hffb395be3233)
{
  int32_t hxc_tmp_enum_switch_result_n1 = 0;
  switch (hxc_value_hffb395be3233.hxc_tag) {
    case hxc_Option_None_hdcfb48028a4b:
      {
        hxc_tmp_enum_switch_result_n1 = 0;
        break;
      }
    case hxc_Option_Some_ha8dd5a59e40a:
      {
        if (hxc_value_hffb395be3233.hxc_tag != hxc_Option_Some_ha8dd5a59e40a)
        {
          abort();
        }
        struct hxc_Rule hxc_tmp_enum_payload_project_n0 = hxc_value_hffb395be3233.hxc_payload.hxc_Some.hxc_value;
        struct hxc_Rule hxc_value_hcf71bd05628a = hxc_tmp_enum_payload_project_n0;
        if (hxc_record_9f230b68_retain(&hxc_value_hcf71bd05628a) != HXC_STATUS_OK)
        {
          abort();
        }
        struct hxc_Rule hxc_rule = hxc_value_hcf71bd05628a;
        if (hxc_record_9f230b68_retain(&hxc_rule) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_tmp_call_result_n3 = hxc_EnumFixture_ruleValue(hxc_rule);
        hxc_tmp_enum_switch_result_n1 = hxc_tmp_call_result_n3;
        hxc_record_9f230b68_destroy(&hxc_rule);
        hxc_record_9f230b68_destroy(&hxc_value_hcf71bd05628a);
        break;
      }
  }
  return hxc_tmp_enum_switch_result_n1;
}

int32_t hxc_EnumFixture_recursiveLocal(void)
{
  struct hxc_Chain hxc_tail = (struct hxc_Chain){ .hxc_tag = hxc_Chain_End, .hxc_payload.hxc_End.hxc_value = 2 };
  struct hxc_Chain hxc_tmp_enum_payload_1_owner_n2 = hxc_tail;
  if (hxc_enum_39285fe9_retain(&hxc_tmp_enum_payload_1_owner_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_Chain hxc_tmp_enum_payload_1_owned_load_result_n2 = hxc_tmp_enum_payload_1_owner_n2;
  struct hxc_Chain *hxc_tmp_enum_recursive_payload_owner_n3 = NULL;
  hxc_allocator hxc_tmp_enum_recursive_payload_owner_n3_allocator = hxc_default_allocator();
  if (hxc_alloc(&hxc_tmp_enum_recursive_payload_owner_n3_allocator, sizeof(struct hxc_Chain), _Alignof(struct hxc_Chain), (void **)&hxc_tmp_enum_recursive_payload_owner_n3) != HXC_STATUS_OK)
  {
    abort();
  }
  *hxc_tmp_enum_recursive_payload_owner_n3 = hxc_tmp_enum_payload_1_owned_load_result_n2;
  struct hxc_Chain hxc_head = (struct hxc_Chain){ .hxc_tag = hxc_Chain_Link, .hxc_payload.hxc_Link.hxc_value = 1, .hxc_payload.hxc_Link.hxc_next = hxc_tmp_enum_recursive_payload_owner_n3 };
  struct hxc_Chain hxc_tmp_load_result_n5 = hxc_head;
  int32_t hxc_tmp_enum_switch_result_n3 = 0;
  switch (hxc_tmp_load_result_n5.hxc_tag) {
    case hxc_Chain_End:
      {
        if (hxc_head.hxc_tag != hxc_Chain_End)
        {
          abort();
        }
        int32_t hxc_tmp_enum_payload_project_n7 = hxc_head.hxc_payload.hxc_End.hxc_value;
        int32_t hxc_value_h34f86739fe15 = hxc_tmp_enum_payload_project_n7;
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
        int32_t hxc_tmp_enum_payload_project_n11 = hxc_head.hxc_payload.hxc_Link.hxc_value;
        int32_t hxc_value_h279cf5a91399 = hxc_tmp_enum_payload_project_n11;
        if (hxc_head.hxc_tag != hxc_Chain_Link)
        {
          abort();
        }
        struct hxc_Chain *hxc_tmp_enum_payload_project_n13 = hxc_head.hxc_payload.hxc_Link.hxc_next;
        struct hxc_Chain hxc_tmp_enum_recursive_payload_load_result_n14 = *hxc_tmp_enum_payload_project_n13;
        struct hxc_Chain hxc_next_h07fb61766894 = hxc_tmp_enum_recursive_payload_load_result_n14;
        if (hxc_enum_39285fe9_retain(&hxc_next_h07fb61766894) != HXC_STATUS_OK)
        {
          abort();
        }
        int32_t hxc_value_hba17dd6c799c = hxc_value_h279cf5a91399;
        struct hxc_Chain hxc_next_he440e694a9fe = hxc_next_h07fb61766894;
        if (hxc_enum_39285fe9_retain(&hxc_next_he440e694a9fe) != HXC_STATUS_OK)
        {
          abort();
        }
        struct hxc_Chain hxc_tmp_load_result_n17 = hxc_next_he440e694a9fe;
        int32_t hxc_tmp_enum_switch_result_n10 = 0;
        switch (hxc_tmp_load_result_n17.hxc_tag) {
          case hxc_Chain_End:
            {
              if (hxc_next_he440e694a9fe.hxc_tag != hxc_Chain_End)
              {
                abort();
              }
              int32_t hxc_tmp_enum_payload_project_n19 = hxc_next_he440e694a9fe.hxc_payload.hxc_End.hxc_value;
              int32_t hxc_value_h443e3a509706 = hxc_tmp_enum_payload_project_n19;
              int32_t hxc_last = hxc_value_h443e3a509706;
              int32_t hxc_tmp_load_result_n21 = hxc_value_hba17dd6c799c;
              hxc_tmp_enum_switch_result_n10 = hxc_i32_add_wrapping(hxc_tmp_load_result_n21, hxc_last);
              break;
            }
          case hxc_Chain_Link:
            {
              hxc_tmp_enum_switch_result_n10 = 0;
              break;
            }
        }
        hxc_tmp_enum_switch_result_n3 = hxc_tmp_enum_switch_result_n10;
        hxc_enum_39285fe9_destroy(&hxc_next_he440e694a9fe);
        hxc_enum_39285fe9_destroy(&hxc_next_h07fb61766894);
        break;
      }
  }
  int32_t hxc_tmp_enum_switch_result_load_result_n24 = hxc_tmp_enum_switch_result_n3;
  hxc_enum_39285fe9_destroy(&hxc_head);
  hxc_enum_39285fe9_destroy(&hxc_tail);
  return hxc_tmp_enum_switch_result_load_result_n24;
}

int32_t hxc_EnumFixture_ruleValue(struct hxc_Rule hxc_value)
{
  int32_t hxc_tmp_call_result_n1 = hxc_EnumFixture_chainValue(hxc_value.hxc_chain);
  int32_t hxc_tmp_call_result_n3 = hxc_EnumFixture_choiceValue(hxc_value.hxc_choices);
  int32_t hxc_tmp_array_get_result_n5;
  if (hxc_array_ref_get_copy(hxc_value.hxc_actions, (size_t)0, &hxc_tmp_array_get_result_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_tmp_call_result_n1, hxc_tmp_call_result_n3), hxc_tmp_array_get_result_n5);
}

int32_t hxc_EnumFixture_tailValue(struct hxc_Chain hxc_value_h43dc2cb9ec11)
{
  int32_t hxc_tmp_enum_switch_result_n1 = 0;
  switch (hxc_value_h43dc2cb9ec11.hxc_tag) {
    case hxc_Chain_End:
      {
        if (hxc_value_h43dc2cb9ec11.hxc_tag != hxc_Chain_End)
        {
          abort();
        }
        int32_t hxc_tmp_enum_payload_project_n0 = hxc_value_h43dc2cb9ec11.hxc_payload.hxc_End.hxc_value;
        int32_t hxc_value_hce7cb8e32020 = hxc_tmp_enum_payload_project_n0;
        int32_t hxc_item = hxc_value_hce7cb8e32020;
        hxc_tmp_enum_switch_result_n1 = hxc_item;
        break;
      }
    case hxc_Chain_Link:
      {
        hxc_tmp_enum_switch_result_n1 = 0;
        break;
      }
  }
  return hxc_tmp_enum_switch_result_n1;
}

struct hxc_RuleEnvelope hxc_EnumFixture_wrapRule(struct hxc_Rule hxc_value)
{
  struct hxc_Rule hxc_tmp_enum_payload_0_owner_n1 = hxc_value;
  if (hxc_record_9f230b68_retain(&hxc_tmp_enum_payload_0_owner_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_RuleEnvelope){ .hxc_tag = hxc_RuleEnvelope_WrappedRule, .hxc_payload.hxc_WrappedRule.hxc_rule = hxc_tmp_enum_payload_0_owner_n1 };
}

struct hxc_Option_h95f1c4a28dac hxc_Option_i32_Some_adapter(int32_t hxc_value)
{
  return (struct hxc_Option_h95f1c4a28dac){ .hxc_tag = hxc_Option_Some_ha9454146ff01, .hxc_payload.hxc_Some.hxc_value = hxc_value };
}

int main(void)
{
  hxc_EnumFixture_main();
  return 0;
}
