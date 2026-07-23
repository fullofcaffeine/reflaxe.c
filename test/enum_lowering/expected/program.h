#ifndef HXC_PROGRAM_H_INCLUDED
#define HXC_PROGRAM_H_INCLUDED

#include <hxrt/allocator.h>
#include <hxrt/array.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef char hxc_runtime_abi_major_must_match[HXC_RUNTIME_ABI_MAJOR == 0U ? 1 : -1];

static inline int32_t hxc_u32_to_i32_bits(uint32_t hxc_value)
{
  if (hxc_value <= UINT32_C(2147483647))
  {
    return (int32_t)hxc_value;
  }
  return INT32_MIN + (int32_t)(hxc_value - UINT32_C(2147483648));
}

static inline int32_t hxc_i32_add_wrapping(int32_t hxc_left, int32_t hxc_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_left + (uint64_t)(uint32_t)hxc_right));
}

hxc_status hxc_record_9f230b68_retain(void *hxc_value);

void hxc_record_9f230b68_destroy(void *hxc_value);

hxc_status hxc_enum_24936704_retain(void *hxc_value);

void hxc_enum_24936704_destroy(void *hxc_value);

hxc_status hxc_enum_39285fe9_retain(void *hxc_value);

void hxc_enum_39285fe9_destroy(void *hxc_value);

hxc_status hxc_enum_39285fe9_retain_recursive_clone(void *hxc_enum_39285fe9_retain_recursive_clone_slot);

void hxc_enum_39285fe9_destroy_recursive_destroy(void *hxc_enum_39285fe9_destroy_recursive_destroy_slot);

hxc_status hxc_enum_d215f611_retain(void *hxc_value);

void hxc_enum_d215f611_destroy(void *hxc_value);

hxc_status hxc_enum_ffce8027_retain(void *hxc_value);

void hxc_enum_ffce8027_destroy(void *hxc_value);

hxc_status hxc_array_400559e4_element_copy(void *hxc_context, void *hxc_destination, const void *hxc_source);

hxc_status hxc_array_400559e4_element_assign(void *hxc_context, void *hxc_destination, const void *hxc_source);

void hxc_array_400559e4_element_destroy(void *hxc_context, void *hxc_element);

hxc_status hxc_array_84c38722_element_copy(void *hxc_context, void *hxc_destination, const void *hxc_source);

hxc_status hxc_array_84c38722_element_assign(void *hxc_context, void *hxc_destination, const void *hxc_source);

void hxc_array_84c38722_element_destroy(void *hxc_context, void *hxc_element);

struct hxc_Option_ha0e4b5dcc139;

struct hxc_Option_h2a07afaff02e;

struct hxc_Option_h95f1c4a28dac;

struct hxc_Chain;

struct hxc_Choices;

struct hxc_RuleEnvelope;

enum hxc_Chain_tag {
  hxc_Chain_End = 0,
  hxc_Chain_Link = 1
};

struct hxc_Chain_End_payload {
  int32_t hxc_value;
};

struct hxc_Chain_Link_payload {
  int32_t hxc_value;
  struct hxc_Chain *hxc_next;
};

union hxc_Chain_payload {
  struct hxc_Chain_End_payload hxc_End;
  struct hxc_Chain_Link_payload hxc_Link;
};

struct hxc_Chain {
  enum hxc_Chain_tag hxc_tag;
  union hxc_Chain_payload hxc_payload;
};

enum hxc_Choices_tag {
  hxc_Choices_NoChoices = 0,
  hxc_Choices_ChoiceValues = 1
};

struct hxc_Choices_ChoiceValues_payload {
  struct hxc_array_ref *hxc_values;
};

union hxc_Choices_payload {
  struct hxc_Choices_ChoiceValues_payload hxc_ChoiceValues;
};

struct hxc_Choices {
  enum hxc_Choices_tag hxc_tag;
  union hxc_Choices_payload hxc_payload;
};

struct hxc_Rule {
  struct hxc_array_ref *hxc_actions;
  struct hxc_Chain hxc_chain;
  struct hxc_Choices hxc_choices;
};

enum hxc_Option_tag_h4f842caea9db {
  hxc_Option_None_h00cd578bb80f = 0,
  hxc_Option_Some_h33493695ace2 = 1
};

struct hxc_Option_Some_payload_hc2b8e9073c59 {
  bool hxc_value;
};

union hxc_Option_payload_ha68af457b79c {
  struct hxc_Option_Some_payload_hc2b8e9073c59 hxc_Some;
};

struct hxc_Option_ha0e4b5dcc139 {
  enum hxc_Option_tag_h4f842caea9db hxc_tag;
  union hxc_Option_payload_ha68af457b79c hxc_payload;
};

enum hxc_Option_tag_hff067ac061db {
  hxc_Option_None_hdcfb48028a4b = 0,
  hxc_Option_Some_ha8dd5a59e40a = 1
};

struct hxc_Option_Some_payload_h663331e4468e {
  struct hxc_Rule hxc_value;
};

union hxc_Option_payload_hbc7d11cfb27e {
  struct hxc_Option_Some_payload_h663331e4468e hxc_Some;
};

struct hxc_Option_h2a07afaff02e {
  enum hxc_Option_tag_hff067ac061db hxc_tag;
  union hxc_Option_payload_hbc7d11cfb27e hxc_payload;
};

enum hxc_Option_tag_h51b3904815c1 {
  hxc_Option_None_h506b5e6013bd = 0,
  hxc_Option_Some_ha9454146ff01 = 1
};

struct hxc_Option_Some_payload_h6fa8fca385dc {
  int32_t hxc_value;
};

union hxc_Option_payload_h331368fdb4fc {
  struct hxc_Option_Some_payload_h6fa8fca385dc hxc_Some;
};

struct hxc_Option_h95f1c4a28dac {
  enum hxc_Option_tag_h51b3904815c1 hxc_tag;
  union hxc_Option_payload_h331368fdb4fc hxc_payload;
};

enum hxc_Mode {
  hxc_Mode_Off = 0,
  hxc_Mode_On = 1
};

enum hxc_RuleEnvelope_tag {
  hxc_RuleEnvelope_MissingRule = 0,
  hxc_RuleEnvelope_WrappedRule = 1
};

struct hxc_RuleEnvelope_WrappedRule_payload {
  struct hxc_Rule hxc_rule;
};

union hxc_RuleEnvelope_payload {
  struct hxc_RuleEnvelope_WrappedRule_payload hxc_WrappedRule;
};

struct hxc_RuleEnvelope {
  enum hxc_RuleEnvelope_tag hxc_tag;
  union hxc_RuleEnvelope_payload hxc_payload;
};

struct hxc_Option_h95f1c4a28dac hxc_EnumFixture_applyOption(int32_t hxc_value, struct hxc_Option_h95f1c4a28dac (*hxc_constructor)(int32_t));

int32_t hxc_EnumFixture_boolOptionValue(struct hxc_Option_ha0e4b5dcc139 hxc_value_hb6e6538779c8);

int32_t hxc_EnumFixture_chainValue(struct hxc_Chain hxc_value_h10794fed7059);

int32_t hxc_EnumFixture_choiceValue(struct hxc_Choices hxc_value);

int32_t hxc_EnumFixture_constructorValue(void);

struct hxc_RuleEnvelope hxc_EnumFixture_copyEnvelope(struct hxc_RuleEnvelope hxc_value);

struct hxc_Rule hxc_EnumFixture_copyRule(struct hxc_Rule hxc_value);

int32_t hxc_EnumFixture_envelopeValue(struct hxc_RuleEnvelope hxc_value);

int32_t hxc_EnumFixture_guardedValue(struct hxc_Option_h95f1c4a28dac hxc_value_ha201421511a7);

int32_t hxc_EnumFixture_identity(int32_t hxc_value);

enum hxc_Mode hxc_EnumFixture_identityMode(enum hxc_Mode hxc_value);

void hxc_EnumFixture_main(void);

struct hxc_Rule hxc_EnumFixture_makeRule(struct hxc_Chain hxc_chain, struct hxc_Choices hxc_choices, struct hxc_array_ref *hxc_actions);

bool hxc_EnumFixture_modeEquality(void);

int32_t hxc_EnumFixture_modeValue(enum hxc_Mode hxc_value);

int32_t hxc_EnumFixture_optionValue(struct hxc_Option_h95f1c4a28dac hxc_value_h2c5c76013588);

int32_t hxc_EnumFixture_optionalRuleValue(struct hxc_Option_h2a07afaff02e hxc_value_hffb395be3233);

int32_t hxc_EnumFixture_recursiveLocal(void);

int32_t hxc_EnumFixture_ruleValue(struct hxc_Rule hxc_value);

int32_t hxc_EnumFixture_tailValue(struct hxc_Chain hxc_value_h43dc2cb9ec11);

struct hxc_RuleEnvelope hxc_EnumFixture_wrapRule(struct hxc_Rule hxc_value);

struct hxc_Option_h95f1c4a28dac hxc_Option_i32_Some_adapter(int32_t hxc_value);

#endif /* HXC_PROGRAM_H_INCLUDED */
