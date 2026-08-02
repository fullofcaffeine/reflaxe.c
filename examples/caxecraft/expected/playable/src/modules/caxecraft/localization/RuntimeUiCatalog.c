#include "hxc/program.h"

void hxc_compiler_constructor_caxecraft_localization_RuntimeUiCatalog(struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_self, hxc_string hxc_l_catalogId, struct hxc_array_ref *hxc_l_locales, struct hxc_array_ref *hxc_l_messages)
{
  const void *hxc_l_gc_roots[2] = { (const void *)hxc_l_self, (const void *)hxc_l_messages };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 2, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_TField_catalogIdValue_managed_String_initializer_owner_n4 = hxc_l_catalogId;
  if (hxc_string_retain(hxc_l_tmp_TField_catalogIdValue_managed_String_initializer_owner_n4) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_catalogIdValue = hxc_l_tmp_TField_catalogIdValue_managed_String_initializer_owner_n4;
  (*hxc_l_self).hxc_locales = hxc_l_locales;
  if (hxc_array_ref_retain((*hxc_l_self).hxc_locales) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_messages = hxc_l_messages;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_compiler_constructor_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition(struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *hxc_l_self, hxc_string hxc_l_id, hxc_string hxc_l_symbol, struct hxc_array_ref *hxc_l_texts)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_TField_id_managed_String_initializer_owner_n4 = hxc_l_id;
  if (hxc_string_retain(hxc_l_tmp_TField_id_managed_String_initializer_owner_n4) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_id = hxc_l_tmp_TField_id_managed_String_initializer_owner_n4;
  hxc_string hxc_l_tmp_TField_symbol_managed_String_initializer_owner_n5 = hxc_l_symbol;
  if (hxc_string_retain(hxc_l_tmp_TField_symbol_managed_String_initializer_owner_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_symbol = hxc_l_tmp_TField_symbol_managed_String_initializer_owner_n5;
  (*hxc_l_self).hxc_texts = hxc_l_texts;
  if (hxc_array_ref_retain((*hxc_l_self).hxc_texts) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_caxecraft_localization_RuntimeUiCatalog_decode(struct hxc_bytes_ref *hxc_l_input)
{
  const void *hxc_l_gc_roots[108] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 108, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_enum_payload_0_owner_n11 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n12 = { 0 };
  hxc_string hxc_l_catalogId = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n15 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n16 = { 0 };
  struct hxc_array_ref *hxc_l_locales = { 0 };
  hxc_string hxc_l_value = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n24 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n25 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n26 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n27 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n28 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n29 = { 0 };
  hxc_string hxc_l_existing_h76dca3e3b8aa = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n32 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n33 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n34 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n35 = { 0 };
  hxc_string hxc_l_defaultLocale = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n39 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n40 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n41 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_static_call_argument_1_owner_n42 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n43 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n44 = { 0 };
  struct hxc_array_ref *hxc_l_expectedIds = { 0 };
  struct hxc_array_ref *hxc_l_expectedSymbols = { 0 };
  struct hxc_array_ref *hxc_l_tmp_instance_call_argument_2_owner_n5 = { 0 };
  hxc_string hxc_l_path = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n54 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n55 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_instance_call_argument_2_owner_n57 = { 0 };
  hxc_string hxc_l_id = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n61 = { 0 };
  hxc_string hxc_l_symbol = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n63 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n65 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n66 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n70 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_1_owner_n71 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n72 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n74 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n75 = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n77 = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n79 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n80 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n81 = { 0 };
  struct hxc_array_ref *hxc_l_texts = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n83 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n84 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n85 = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n89 = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n91 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n92 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n93 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n94 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaReader hxc_l_tmp_object_storage_n2 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_class_object_address_n0 = &hxc_l_tmp_object_storage_n2;
  hxc_compiler_constructor_caxecraft_content_RuntimeSchemaReader(hxc_l_tmp_class_object_address_n0);
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader = hxc_l_tmp_class_object_address_n0;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n1 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n1 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n2 = hxc_caxecraft_content_RuntimeSchemaReader_parse(hxc_l_tmp_load_result_n1, hxc_l_input);
  hxc_l_gc_roots[0] = (const void *)hxc_l_tmp_instance_call_result_n2;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_root = hxc_l_tmp_instance_call_result_n2;
  hxc_l_gc_roots[1] = (const void *)hxc_l_root;
  hxc_l_gc_roots[2] = (const void *)NULL;
  if (hxc_l_root == NULL)
  {
    struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n5 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
    hxc_l_gc_roots[3] = hxc_l_tmp_call_result_n5.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n5.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n5;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n6 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n6 == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n7 = hxc_l_root;
  hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n7;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n8 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n8) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"schemaVersion", 13, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"catalogId", 9, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"defaultLocale", 13, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"locales", 7, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"messages", 8, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_instance_call_argument_2_owner_n5 = hxc_l_tmp_array_create_result_n8;
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n10 = hxc_caxecraft_content_RuntimeSchemaReader_object(hxc_l_tmp_load_result_n6, hxc_l_tmp_load_result_n7, (hxc_string){ (const uint8_t *)"UI catalog", 10, true, NULL }, hxc_l_tmp_instance_call_argument_2_owner_n5);
  hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_instance_call_result_n10;
  struct hxc_array_ref *hxc_l_fields = hxc_l_tmp_instance_call_result_n10;
  hxc_l_gc_roots[6] = (const void *)hxc_l_fields;
  hxc_l_gc_roots[7] = (const void *)NULL;
  if (hxc_l_fields == NULL)
  {
    struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n13 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
    hxc_l_gc_roots[8] = hxc_l_tmp_call_result_n13.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n13.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n13;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n14 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n14 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[9] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n16 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n14, hxc_l_fields, (hxc_string){ (const uint8_t *)"schemaVersion", 13, true, NULL });
  hxc_l_gc_roots[10] = (const void *)hxc_l_tmp_instance_call_result_n16;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_versionNode = hxc_l_tmp_instance_call_result_n16;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n17 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n17 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[11] = (const void *)hxc_l_versionNode;
  struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_instance_call_result_n19 = hxc_caxecraft_content_RuntimeSchemaReader_integer(hxc_l_tmp_load_result_n17, hxc_l_versionNode, (hxc_string){ (const uint8_t *)"schemaVersion", 13, true, NULL }, 0, 2147483647);
  struct hxc_optional_value_h76f8e80e9d2b hxc_l_version = hxc_l_tmp_instance_call_result_n19;
  if (!hxc_l_version.hxc_has_value)
  {
    struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n22 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
    hxc_l_gc_roots[12] = hxc_l_tmp_call_result_n22.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n22.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n22;
  }
  struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_optional_scalar_equality_optional_n8 = hxc_l_version;
  int32_t hxc_l_tmp_optional_scalar_equality_scalar_n9 = 1;
  bool hxc_l_tmp_optional_scalar_equality_result_n10 = true;
  if (hxc_l_tmp_optional_scalar_equality_optional_n8.hxc_has_value)
  {
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_optional_scalar_compare_optional_load_result_n25 = hxc_l_tmp_optional_scalar_equality_optional_n8;
    int32_t hxc_l_tmp_optional_scalar_compare_scalar_load_result_n26 = hxc_l_tmp_optional_scalar_equality_scalar_n9;
    if (!hxc_l_tmp_optional_scalar_compare_optional_load_result_n25.hxc_has_value)
    {
      abort();
    }
    hxc_l_tmp_optional_scalar_equality_result_n10 = hxc_l_tmp_optional_scalar_compare_optional_load_result_n25.hxc_value != hxc_l_tmp_optional_scalar_compare_scalar_load_result_n26;
  }
  if (hxc_l_tmp_optional_scalar_equality_result_n10)
  {
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n28 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n28 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n29 = hxc_l_versionNode;
    hxc_l_gc_roots[13] = (const void *)hxc_l_tmp_load_result_n29;
    hxc_l_tmp_enum_payload_0_owner_n11 = (hxc_string){ (const uint8_t *)"schemaVersion", 13, true, NULL };
    if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n11) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n12 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaUnsupportedVersion, .hxc_payload.hxc_SchemaUnsupportedVersion.hxc_path = hxc_l_tmp_enum_payload_0_owner_n11, .hxc_payload.hxc_SchemaUnsupportedVersion.hxc_expected = 1 };
    hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n28, hxc_l_tmp_load_result_n29, hxc_l_tmp_instance_call_argument_1_owner_n12);
    struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n34 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
    hxc_l_gc_roots[14] = hxc_l_tmp_call_result_n34.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n34.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
    hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n12);
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n34;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n35 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n35 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[15] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n37 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n35, hxc_l_fields, (hxc_string){ (const uint8_t *)"catalogId", 9, true, NULL });
  hxc_l_gc_roots[16] = (const void *)hxc_l_tmp_instance_call_result_n37;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_catalogNode = hxc_l_tmp_instance_call_result_n37;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n38 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n38 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[17] = (const void *)hxc_l_catalogNode;
  hxc_string hxc_l_tmp_instance_call_result_n40 = hxc_caxecraft_content_RuntimeSchemaReader_string(hxc_l_tmp_load_result_n38, hxc_l_catalogNode, (hxc_string){ (const uint8_t *)"catalogId", 9, true, NULL }, 128);
  hxc_l_catalogId = hxc_l_tmp_instance_call_result_n40;
  if (hxc_l_catalogId.data == NULL)
  {
    struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n43 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
    hxc_l_gc_roots[18] = hxc_l_tmp_call_result_n43.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n43.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
    if (hxc_string_release(&hxc_l_catalogId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n43;
  }
  if (!(hxc_l_catalogId.data != NULL && (hxc_l_catalogId.byte_length == (hxc_string){ (const uint8_t *)"caxecraft.ui", 12, true, NULL }.byte_length && (hxc_l_catalogId.byte_length == 0 || memcmp(hxc_l_catalogId.data, (hxc_string){ (const uint8_t *)"caxecraft.ui", 12, true, NULL }.data, hxc_l_catalogId.byte_length) == 0))))
  {
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n46 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n46 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n47 = hxc_l_catalogNode;
    hxc_l_gc_roots[19] = (const void *)hxc_l_tmp_load_result_n47;
    hxc_l_tmp_enum_payload_0_owner_n15 = (hxc_string){ (const uint8_t *)"catalogId", 9, true, NULL };
    if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n15) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n16 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaIncompatibleTypedCatalog, .hxc_payload.hxc_SchemaIncompatibleTypedCatalog.hxc_path = hxc_l_tmp_enum_payload_0_owner_n15 };
    hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n46, hxc_l_tmp_load_result_n47, hxc_l_tmp_instance_call_argument_1_owner_n16);
    struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n52 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
    hxc_l_gc_roots[20] = hxc_l_tmp_call_result_n52.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n52.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
    hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n16);
    if (hxc_string_release(&hxc_l_catalogId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n52;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n53 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n53 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[21] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n55 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n53, hxc_l_fields, (hxc_string){ (const uint8_t *)"locales", 7, true, NULL });
  hxc_l_gc_roots[22] = (const void *)hxc_l_tmp_instance_call_result_n55;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_localeNode = hxc_l_tmp_instance_call_result_n55;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n56 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n56 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[23] = (const void *)hxc_l_localeNode;
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n58 = hxc_caxecraft_content_RuntimeSchemaReader_array(hxc_l_tmp_load_result_n56, hxc_l_localeNode, (hxc_string){ (const uint8_t *)"locales", 7, true, NULL }, 1, 8);
  hxc_l_gc_roots[24] = (const void *)hxc_l_tmp_instance_call_result_n58;
  struct hxc_array_ref *hxc_l_localeValues = hxc_l_tmp_instance_call_result_n58;
  hxc_l_gc_roots[25] = (const void *)hxc_l_localeValues;
  hxc_l_gc_roots[26] = (const void *)NULL;
  if (hxc_l_localeValues == NULL)
  {
    struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n61 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
    hxc_l_gc_roots[27] = hxc_l_tmp_call_result_n61.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n61.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
    if (hxc_string_release(&hxc_l_catalogId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n61;
  }
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n62 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n62) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_locales = hxc_l_tmp_array_create_result_n62;
  int32_t hxc_l_g_hd68b7e6b80bd = 0;
  hxc_l_gc_roots[28] = (const void *)hxc_l_localeValues;
  int32_t hxc_l_tmp_array_length_result_n64;
  if (hxc_array_ref_length(hxc_l_localeValues, &hxc_l_tmp_array_length_result_n64) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1_hfe60c0970d6a = hxc_l_tmp_array_length_result_n64;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n65 = hxc_l_g_hd68b7e6b80bd;
    if (!(hxc_l_tmp_load_result_n65 < hxc_l_g1_hfe60c0970d6a))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n67 = hxc_l_g_hd68b7e6b80bd;
    hxc_l_g_hd68b7e6b80bd = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n67, 1);
    int32_t hxc_l_index_h9f585ea65a19 = hxc_l_tmp_increment_load_result_n67;
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n68 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n68 == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n69 = hxc_l_localeValues;
    hxc_l_gc_roots[29] = (const void *)hxc_l_tmp_load_result_n69;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_array_get_result_n71;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n69, (size_t)hxc_l_index_h9f585ea65a19, &hxc_l_tmp_array_get_result_n71) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[30] = (const void *)hxc_l_tmp_array_get_result_n71;
    hxc_string hxc_l_tmp_std_string_int_result_n73 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_from_int32(hxc_l_index_h9f585ea65a19, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n73) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_right_owner_n24 = hxc_l_tmp_std_string_int_result_n73;
    hxc_string hxc_l_tmp_string_concat_result_n75 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"locales[", 8, true, NULL }, hxc_l_tmp_string_concat_right_owner_n24, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n75) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_left_owner_n25 = hxc_l_tmp_string_concat_result_n75;
    hxc_string hxc_l_tmp_string_concat_result_n77 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n25, (hxc_string){ (const uint8_t *)"]", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n77) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n26 = hxc_l_tmp_string_concat_result_n77;
    hxc_string hxc_l_tmp_instance_call_result_n79 = hxc_caxecraft_content_RuntimeSchemaReader_string(hxc_l_tmp_load_result_n68, hxc_l_tmp_array_get_result_n71, hxc_l_tmp_instance_call_argument_1_owner_n26, 128);
    hxc_l_value = hxc_l_tmp_instance_call_result_n79;
    if (hxc_l_value.data == NULL)
    {
      struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n82 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
      hxc_l_gc_roots[34] = hxc_l_tmp_call_result_n82.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n82.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
      if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n26) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n25) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n24) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_locales) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_catalogId) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_tmp_call_result_n82;
    }
    bool hxc_l_tmp_call_result_n84 = hxc_caxecraft_content_RuntimeSchemaReader_validLocale(hxc_l_value);
    if (!hxc_l_tmp_call_result_n84)
    {
      struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n85 = hxc_l_reader;
      if (hxc_l_tmp_load_result_n85 == NULL)
      {
        abort();
      }
      struct hxc_array_ref *hxc_l_tmp_load_result_n86 = hxc_l_localeValues;
      hxc_l_gc_roots[35] = (const void *)hxc_l_tmp_load_result_n86;
      struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_array_get_result_n88;
      if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n86, (size_t)hxc_l_index_h9f585ea65a19, &hxc_l_tmp_array_get_result_n88) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_gc_roots[36] = (const void *)hxc_l_tmp_array_get_result_n88;
      hxc_string hxc_l_tmp_std_string_int_result_n90 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_from_int32(hxc_l_index_h9f585ea65a19, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n90) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_string_concat_right_owner_n27 = hxc_l_tmp_std_string_int_result_n90;
      hxc_string hxc_l_tmp_string_concat_result_n92 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"locales[", 8, true, NULL }, hxc_l_tmp_string_concat_right_owner_n27, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n92) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_string_concat_left_owner_n28 = hxc_l_tmp_string_concat_result_n92;
      hxc_string hxc_l_tmp_string_concat_result_n94 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n28, (hxc_string){ (const uint8_t *)"]", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n94) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_instance_call_argument_1_owner_n29 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidLocale, .hxc_payload.hxc_SchemaInvalidLocale.hxc_path = hxc_l_tmp_string_concat_result_n94 };
      hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n85, hxc_l_tmp_array_get_result_n88, hxc_l_tmp_instance_call_argument_1_owner_n29);
      struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n98 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
      hxc_l_gc_roots[37] = hxc_l_tmp_call_result_n98.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n98.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
      hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n29);
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n28) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n27) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n26) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n25) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n24) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_locales) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_catalogId) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_tmp_call_result_n98;
    }
    int32_t hxc_l_g_he1c133838fc7 = 0;
    while (1)
    {
      int32_t hxc_l_tmp_load_result_n99 = hxc_l_g_he1c133838fc7;
      int32_t hxc_l_tmp_array_length_result_n101;
      if (hxc_array_ref_length(hxc_l_locales, &hxc_l_tmp_array_length_result_n101) != HXC_STATUS_OK)
      {
        abort();
      }
      if (!(hxc_l_tmp_load_result_n99 < hxc_l_tmp_array_length_result_n101))
      {
        break;
      }
      struct hxc_array_ref *hxc_l_tmp_load_result_n102 = hxc_l_locales;
      hxc_string hxc_l_tmp_array_get_result_n104;
      if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n102, (size_t)hxc_l_g_he1c133838fc7, &hxc_l_tmp_array_get_result_n104) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_array_element_owner_n32 = hxc_l_tmp_array_get_result_n104;
      hxc_l_existing_h76dca3e3b8aa = hxc_l_tmp_array_element_owner_n32;
      if (hxc_string_retain(hxc_l_existing_h76dca3e3b8aa) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_g_he1c133838fc7 = hxc_i32_add_wrapping(hxc_l_g_he1c133838fc7, 1);
      hxc_string hxc_l_tmp_load_result_n107 = hxc_l_existing_h76dca3e3b8aa;
      if (hxc_l_tmp_load_result_n107.data == NULL || hxc_l_value.data == NULL ? hxc_l_tmp_load_result_n107.data == hxc_l_value.data : hxc_l_tmp_load_result_n107.byte_length == hxc_l_value.byte_length && (hxc_l_tmp_load_result_n107.byte_length == 0 || memcmp(hxc_l_tmp_load_result_n107.data, hxc_l_value.data, hxc_l_tmp_load_result_n107.byte_length) == 0))
      {
        struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n110 = hxc_l_reader;
        if (hxc_l_tmp_load_result_n110 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_load_result_n111 = hxc_l_localeValues;
        hxc_l_gc_roots[38] = (const void *)hxc_l_tmp_load_result_n111;
        struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_array_get_result_n113;
        if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n111, (size_t)hxc_l_index_h9f585ea65a19, &hxc_l_tmp_array_get_result_n113) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_gc_roots[39] = (const void *)hxc_l_tmp_array_get_result_n113;
        hxc_string hxc_l_tmp_std_string_int_result_n115 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_from_int32(hxc_l_index_h9f585ea65a19, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n115) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_string_concat_right_owner_n33 = hxc_l_tmp_std_string_int_result_n115;
        hxc_string hxc_l_tmp_string_concat_result_n117 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"locales[", 8, true, NULL }, hxc_l_tmp_string_concat_right_owner_n33, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n117) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_string_concat_left_owner_n34 = hxc_l_tmp_string_concat_result_n117;
        hxc_string hxc_l_tmp_string_concat_result_n119 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n34, (hxc_string){ (const uint8_t *)"]", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n119) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_instance_call_argument_1_owner_n35 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidLocale, .hxc_payload.hxc_SchemaInvalidLocale.hxc_path = hxc_l_tmp_string_concat_result_n119 };
        hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n110, hxc_l_tmp_array_get_result_n113, hxc_l_tmp_instance_call_argument_1_owner_n35);
        struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n123 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
        hxc_l_gc_roots[40] = hxc_l_tmp_call_result_n123.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n123.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
        hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n35);
        if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n34) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n33) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_existing_h76dca3e3b8aa) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_array_element_owner_n32) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n26) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n25) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n24) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_array_ref_release(hxc_l_locales) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_catalogId) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
        if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
        {
          abort();
        }
        return hxc_l_tmp_call_result_n123;
      }
      if (hxc_string_release(&hxc_l_existing_h76dca3e3b8aa) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_array_element_owner_n32) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n124 = hxc_l_locales;
    int32_t hxc_l_tmp_array_push_result_n126;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n124, &hxc_l_value, &hxc_l_tmp_array_push_result_n126) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n126;
    if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n26) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n25) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n24) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n127 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n127 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[31] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n129 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n127, hxc_l_fields, (hxc_string){ (const uint8_t *)"defaultLocale", 13, true, NULL });
  hxc_l_gc_roots[32] = (const void *)hxc_l_tmp_instance_call_result_n129;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_defaultNode = hxc_l_tmp_instance_call_result_n129;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n130 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n130 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[33] = (const void *)hxc_l_defaultNode;
  hxc_string hxc_l_tmp_instance_call_result_n132 = hxc_caxecraft_content_RuntimeSchemaReader_string(hxc_l_tmp_load_result_n130, hxc_l_defaultNode, (hxc_string){ (const uint8_t *)"defaultLocale", 13, true, NULL }, 128);
  hxc_l_defaultLocale = hxc_l_tmp_instance_call_result_n132;
  if (hxc_l_defaultLocale.data == NULL)
  {
    struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n135 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
    hxc_l_gc_roots[41] = hxc_l_tmp_call_result_n135.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n135.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
    if (hxc_string_release(&hxc_l_defaultLocale) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_locales) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_catalogId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n135;
  }
  bool hxc_l_tmp_call_result_n137 = hxc_caxecraft_content_RuntimeSchemaReader_validLocale(hxc_l_defaultLocale);
  bool hxc_l_tmp_short_circuit_result_n38 = !hxc_l_tmp_call_result_n137;
  if (!!hxc_l_tmp_call_result_n137)
  {
    hxc_string hxc_l_tmp_load_result_n138 = hxc_l_defaultLocale;
    hxc_string hxc_l_tmp_array_get_result_n140;
    if (hxc_array_ref_get_copy(hxc_l_locales, (size_t)0, &hxc_l_tmp_array_get_result_n140) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n39 = hxc_l_tmp_array_get_result_n140;
    hxc_l_tmp_short_circuit_result_n38 = !(hxc_l_tmp_load_result_n138.data == NULL || hxc_l_tmp_array_element_owner_n39.data == NULL ? hxc_l_tmp_load_result_n138.data == hxc_l_tmp_array_element_owner_n39.data : hxc_l_tmp_load_result_n138.byte_length == hxc_l_tmp_array_element_owner_n39.byte_length && (hxc_l_tmp_load_result_n138.byte_length == 0 || memcmp(hxc_l_tmp_load_result_n138.data, hxc_l_tmp_array_element_owner_n39.data, hxc_l_tmp_load_result_n138.byte_length) == 0));
    if (hxc_string_release(&hxc_l_tmp_array_element_owner_n39) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_tmp_short_circuit_result_n38)
  {
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n144 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n144 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n145 = hxc_l_defaultNode;
    hxc_l_gc_roots[42] = (const void *)hxc_l_tmp_load_result_n145;
    hxc_l_tmp_enum_payload_0_owner_n40 = (hxc_string){ (const uint8_t *)"defaultLocale", 13, true, NULL };
    if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n40) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n41 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidLocale, .hxc_payload.hxc_SchemaInvalidLocale.hxc_path = hxc_l_tmp_enum_payload_0_owner_n40 };
    hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n144, hxc_l_tmp_load_result_n145, hxc_l_tmp_instance_call_argument_1_owner_n41);
    struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n150 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
    hxc_l_gc_roots[43] = hxc_l_tmp_call_result_n150.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n150.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
    hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n41);
    if (hxc_string_release(&hxc_l_defaultLocale) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_locales) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_catalogId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n150;
  }
  struct hxc_array_ref *hxc_l_tmp_load_result_n151 = hxc_l_locales;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n152 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n152) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n152->value, &(hxc_string){ (const uint8_t *)"en", 2, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n152->value, &(hxc_string){ (const uint8_t *)"es-MX", 5, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_static_call_argument_1_owner_n42 = hxc_l_tmp_array_create_result_n152;
  bool hxc_l_tmp_call_result_n154 = hxc_caxecraft_localization_RuntimeUiCatalog_sameStrings(hxc_l_tmp_load_result_n151, hxc_l_tmp_static_call_argument_1_owner_n42);
  if (hxc_array_ref_release(hxc_l_tmp_static_call_argument_1_owner_n42) != HXC_STATUS_OK)
  {
    abort();
  }
  if (!hxc_l_tmp_call_result_n154)
  {
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n155 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n155 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n156 = hxc_l_localeNode;
    hxc_l_gc_roots[44] = (const void *)hxc_l_tmp_load_result_n156;
    hxc_l_tmp_enum_payload_0_owner_n43 = (hxc_string){ (const uint8_t *)"locales", 7, true, NULL };
    if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n43) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n44 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaIncompatibleTypedCatalog, .hxc_payload.hxc_SchemaIncompatibleTypedCatalog.hxc_path = hxc_l_tmp_enum_payload_0_owner_n43 };
    hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n155, hxc_l_tmp_load_result_n156, hxc_l_tmp_instance_call_argument_1_owner_n44);
    struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n161 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
    hxc_l_gc_roots[45] = hxc_l_tmp_call_result_n161.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n161.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
    hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n44);
    if (hxc_string_release(&hxc_l_defaultLocale) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_locales) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_catalogId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n161;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n162 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n162 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[46] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n164 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n162, hxc_l_fields, (hxc_string){ (const uint8_t *)"messages", 8, true, NULL });
  hxc_l_gc_roots[47] = (const void *)hxc_l_tmp_instance_call_result_n164;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_messageNode = hxc_l_tmp_instance_call_result_n164;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n165 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n165 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[48] = (const void *)hxc_l_messageNode;
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n167 = hxc_caxecraft_content_RuntimeSchemaReader_array(hxc_l_tmp_load_result_n165, hxc_l_messageNode, (hxc_string){ (const uint8_t *)"messages", 8, true, NULL }, 1, 128);
  hxc_l_gc_roots[49] = (const void *)hxc_l_tmp_instance_call_result_n167;
  struct hxc_array_ref *hxc_l_messageValues = hxc_l_tmp_instance_call_result_n167;
  hxc_l_gc_roots[50] = (const void *)hxc_l_messageValues;
  hxc_l_gc_roots[51] = (const void *)NULL;
  if (hxc_l_messageValues == NULL)
  {
    struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n170 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
    hxc_l_gc_roots[52] = hxc_l_tmp_call_result_n170.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n170.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
    if (hxc_string_release(&hxc_l_defaultLocale) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_locales) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_catalogId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n170;
  }
  struct hxc_array_ref *hxc_l_tmp_call_result_n171 = hxc_caxecraft_localization_RuntimeUiCatalog_expectedMessageIds();
  hxc_l_expectedIds = hxc_l_tmp_call_result_n171;
  struct hxc_array_ref *hxc_l_tmp_call_result_n172 = hxc_caxecraft_localization_RuntimeUiCatalog_expectedMessageSymbols();
  hxc_l_expectedSymbols = hxc_l_tmp_call_result_n172;
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n173 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_array_4fb4ef37_descriptor, (void **)&hxc_l_tmp_array_create_result_n173) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_init_in_place(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *), _Alignof(struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *), NULL, NULL, NULL, NULL }, hxc_l_tmp_array_create_result_n173) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[53] = (const void *)hxc_l_tmp_array_create_result_n173;
  struct hxc_array_ref *hxc_l_messages = hxc_l_tmp_array_create_result_n173;
  int32_t hxc_l_g_h541c273aca4f = 0;
  hxc_l_gc_roots[54] = (const void *)hxc_l_messageValues;
  int32_t hxc_l_tmp_array_length_result_n175;
  if (hxc_array_ref_length(hxc_l_messageValues, &hxc_l_tmp_array_length_result_n175) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1_ha53039b9910a = hxc_l_tmp_array_length_result_n175;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n176 = hxc_l_g_h541c273aca4f;
    if (!(hxc_l_tmp_load_result_n176 < hxc_l_g1_ha53039b9910a))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n178 = hxc_l_g_h541c273aca4f;
    hxc_l_g_h541c273aca4f = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n178, 1);
    int32_t hxc_l_index_h098f99b4ccb1 = hxc_l_tmp_increment_load_result_n178;
    hxc_string hxc_l_tmp_std_string_int_result_n180 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_from_int32(hxc_l_index_h098f99b4ccb1, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n180) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_right_owner_n54 = hxc_l_tmp_std_string_int_result_n180;
    hxc_string hxc_l_tmp_string_concat_result_n182 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"messages[", 9, true, NULL }, hxc_l_tmp_string_concat_right_owner_n54, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n182) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_left_owner_n55 = hxc_l_tmp_string_concat_result_n182;
    hxc_string hxc_l_tmp_string_concat_result_n184 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n55, (hxc_string){ (const uint8_t *)"]", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n184) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_path = hxc_l_tmp_string_concat_result_n184;
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n185 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n185 == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n186 = hxc_l_messageValues;
    hxc_l_gc_roots[55] = (const void *)hxc_l_tmp_load_result_n186;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_array_get_result_n188;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n186, (size_t)hxc_l_index_h098f99b4ccb1, &hxc_l_tmp_array_get_result_n188) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[56] = (const void *)hxc_l_tmp_array_get_result_n188;
    hxc_string hxc_l_tmp_load_result_n189 = hxc_l_path;
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n190 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n190) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n190->value, &(hxc_string){ (const uint8_t *)"id", 2, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n190->value, &(hxc_string){ (const uint8_t *)"symbol", 6, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n190->value, &(hxc_string){ (const uint8_t *)"text", 4, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_2_owner_n57 = hxc_l_tmp_array_create_result_n190;
    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n192 = hxc_caxecraft_content_RuntimeSchemaReader_object(hxc_l_tmp_load_result_n185, hxc_l_tmp_array_get_result_n188, hxc_l_tmp_load_result_n189, hxc_l_tmp_instance_call_argument_2_owner_n57);
    hxc_l_gc_roots[57] = (const void *)hxc_l_tmp_instance_call_result_n192;
    struct hxc_array_ref *hxc_l_messageFields = hxc_l_tmp_instance_call_result_n192;
    hxc_l_gc_roots[58] = (const void *)hxc_l_messageFields;
    hxc_l_gc_roots[59] = (const void *)NULL;
    if (hxc_l_messageFields == NULL)
    {
      struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n195 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
      hxc_l_gc_roots[61] = hxc_l_tmp_call_result_n195.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n195.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n57) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n55) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n54) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_expectedSymbols) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_expectedIds) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_defaultLocale) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_locales) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_catalogId) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_tmp_call_result_n195;
    }
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n196 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n196 == NULL)
    {
      abort();
    }
    hxc_l_gc_roots[62] = (const void *)hxc_l_messageFields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n198 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n196, hxc_l_messageFields, (hxc_string){ (const uint8_t *)"id", 2, true, NULL });
    hxc_l_gc_roots[63] = (const void *)hxc_l_tmp_instance_call_result_n198;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_idNode = hxc_l_tmp_instance_call_result_n198;
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n199 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n199 == NULL)
    {
      abort();
    }
    hxc_l_gc_roots[64] = (const void *)hxc_l_messageFields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n201 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n199, hxc_l_messageFields, (hxc_string){ (const uint8_t *)"symbol", 6, true, NULL });
    hxc_l_gc_roots[65] = (const void *)hxc_l_tmp_instance_call_result_n201;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_symbolNode = hxc_l_tmp_instance_call_result_n201;
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n202 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n202 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n203 = hxc_l_idNode;
    hxc_l_gc_roots[66] = (const void *)hxc_l_tmp_load_result_n203;
    hxc_string hxc_l_tmp_string_concat_result_n205 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".id", 3, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n205) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n61 = hxc_l_tmp_string_concat_result_n205;
    hxc_string hxc_l_tmp_instance_call_result_n207 = hxc_caxecraft_content_RuntimeSchemaReader_string(hxc_l_tmp_load_result_n202, hxc_l_tmp_load_result_n203, hxc_l_tmp_instance_call_argument_1_owner_n61, 128);
    hxc_l_id = hxc_l_tmp_instance_call_result_n207;
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n208 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n208 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n209 = hxc_l_symbolNode;
    hxc_l_gc_roots[67] = (const void *)hxc_l_tmp_load_result_n209;
    hxc_string hxc_l_tmp_string_concat_result_n211 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".symbol", 7, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n211) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n63 = hxc_l_tmp_string_concat_result_n211;
    hxc_string hxc_l_tmp_instance_call_result_n213 = hxc_caxecraft_content_RuntimeSchemaReader_string(hxc_l_tmp_load_result_n208, hxc_l_tmp_load_result_n209, hxc_l_tmp_instance_call_argument_1_owner_n63, 128);
    hxc_l_symbol = hxc_l_tmp_instance_call_result_n213;
    hxc_string hxc_l_tmp_load_result_n214 = hxc_l_id;
    bool hxc_l_tmp_short_circuit_result_n64 = hxc_l_tmp_load_result_n214.data == NULL;
    if (!(hxc_l_tmp_load_result_n214.data == NULL))
    {
      hxc_l_tmp_short_circuit_result_n64 = hxc_l_symbol.data == NULL;
    }
    if (hxc_l_tmp_short_circuit_result_n64)
    {
      struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n218 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
      hxc_l_gc_roots[68] = hxc_l_tmp_call_result_n218.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n218.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
      if (hxc_string_release(&hxc_l_symbol) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n63) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n61) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n57) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n55) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n54) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_expectedSymbols) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_expectedIds) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_defaultLocale) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_locales) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_catalogId) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_tmp_call_result_n218;
    }
    bool hxc_l_tmp_call_result_n220 = hxc_caxecraft_content_RuntimeSchemaReader_validMessageId(hxc_l_id);
    if (!hxc_l_tmp_call_result_n220)
    {
      struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n221 = hxc_l_reader;
      if (hxc_l_tmp_load_result_n221 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n222 = hxc_l_idNode;
      hxc_l_gc_roots[69] = (const void *)hxc_l_tmp_load_result_n222;
      hxc_string hxc_l_tmp_string_concat_result_n224 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".id", 3, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n224) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_instance_call_argument_1_owner_n65 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidString, .hxc_payload.hxc_SchemaInvalidString.hxc_path = hxc_l_tmp_string_concat_result_n224 };
      hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n221, hxc_l_tmp_load_result_n222, hxc_l_tmp_instance_call_argument_1_owner_n65);
      struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n228 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
      hxc_l_gc_roots[70] = hxc_l_tmp_call_result_n228.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n228.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
      hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n65);
      if (hxc_string_release(&hxc_l_symbol) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n63) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n61) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n57) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n55) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n54) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_expectedSymbols) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_expectedIds) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_defaultLocale) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_locales) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_catalogId) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_tmp_call_result_n228;
    }
    bool hxc_l_tmp_call_result_n230 = hxc_caxecraft_content_RuntimeSchemaReader_validSymbol(hxc_l_symbol);
    if (!hxc_l_tmp_call_result_n230)
    {
      struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n231 = hxc_l_reader;
      if (hxc_l_tmp_load_result_n231 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n232 = hxc_l_symbolNode;
      hxc_l_gc_roots[71] = (const void *)hxc_l_tmp_load_result_n232;
      hxc_string hxc_l_tmp_string_concat_result_n234 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".symbol", 7, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n234) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_instance_call_argument_1_owner_n66 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidString, .hxc_payload.hxc_SchemaInvalidString.hxc_path = hxc_l_tmp_string_concat_result_n234 };
      hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n231, hxc_l_tmp_load_result_n232, hxc_l_tmp_instance_call_argument_1_owner_n66);
      struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n238 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
      hxc_l_gc_roots[72] = hxc_l_tmp_call_result_n238.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n238.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
      hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n66);
      if (hxc_string_release(&hxc_l_symbol) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n63) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n61) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n57) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n55) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n54) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_expectedSymbols) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_expectedIds) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_defaultLocale) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_locales) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_catalogId) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_tmp_call_result_n238;
    }
    int32_t hxc_l_g_h8da555512add = 0;
    while (1)
    {
      int32_t hxc_l_tmp_load_result_n239 = hxc_l_g_h8da555512add;
      hxc_l_gc_roots[73] = (const void *)hxc_l_messages;
      int32_t hxc_l_tmp_array_length_result_n241;
      if (hxc_array_ref_length(hxc_l_messages, &hxc_l_tmp_array_length_result_n241) != HXC_STATUS_OK)
      {
        abort();
      }
      if (!(hxc_l_tmp_load_result_n239 < hxc_l_tmp_array_length_result_n241))
      {
        break;
      }
      struct hxc_array_ref *hxc_l_tmp_load_result_n242 = hxc_l_messages;
      hxc_l_gc_roots[74] = (const void *)hxc_l_tmp_load_result_n242;
      struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *hxc_l_tmp_array_get_result_n244;
      if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n242, (size_t)hxc_l_g_h8da555512add, &hxc_l_tmp_array_get_result_n244) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_gc_roots[75] = (const void *)hxc_l_tmp_array_get_result_n244;
      struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *hxc_l_existing_h9a6fbe78745a = hxc_l_tmp_array_get_result_n244;
      hxc_l_g_h8da555512add = hxc_i32_add_wrapping(hxc_l_g_h8da555512add, 1);
      struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *hxc_l_tmp_load_result_n246 = hxc_l_existing_h9a6fbe78745a;
      hxc_l_gc_roots[76] = (const void *)hxc_l_tmp_load_result_n246;
      if (hxc_l_tmp_load_result_n246 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_class_field_load_result_n247 = (*hxc_l_tmp_load_result_n246).hxc_id;
      hxc_string hxc_l_tmp_load_result_n248 = hxc_l_id;
      bool hxc_l_tmp_short_circuit_result_n69 = hxc_l_tmp_class_field_load_result_n247.data == NULL || hxc_l_tmp_load_result_n248.data == NULL ? hxc_l_tmp_class_field_load_result_n247.data == hxc_l_tmp_load_result_n248.data : hxc_l_tmp_class_field_load_result_n247.byte_length == hxc_l_tmp_load_result_n248.byte_length && (hxc_l_tmp_class_field_load_result_n247.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n247.data, hxc_l_tmp_load_result_n248.data, hxc_l_tmp_class_field_load_result_n247.byte_length) == 0);
      if (!(hxc_l_tmp_class_field_load_result_n247.data == NULL || hxc_l_tmp_load_result_n248.data == NULL ? hxc_l_tmp_class_field_load_result_n247.data == hxc_l_tmp_load_result_n248.data : hxc_l_tmp_class_field_load_result_n247.byte_length == hxc_l_tmp_load_result_n248.byte_length && (hxc_l_tmp_class_field_load_result_n247.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n247.data, hxc_l_tmp_load_result_n248.data, hxc_l_tmp_class_field_load_result_n247.byte_length) == 0)))
      {
        struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *hxc_l_tmp_load_result_n250 = hxc_l_existing_h9a6fbe78745a;
        hxc_l_gc_roots[78] = (const void *)hxc_l_tmp_load_result_n250;
        if (hxc_l_tmp_load_result_n250 == NULL)
        {
          abort();
        }
        hxc_string hxc_l_tmp_class_field_load_result_n251 = (*hxc_l_tmp_load_result_n250).hxc_symbol;
        hxc_l_tmp_short_circuit_result_n69 = hxc_l_tmp_class_field_load_result_n251.data == NULL || hxc_l_symbol.data == NULL ? hxc_l_tmp_class_field_load_result_n251.data == hxc_l_symbol.data : hxc_l_tmp_class_field_load_result_n251.byte_length == hxc_l_symbol.byte_length && (hxc_l_tmp_class_field_load_result_n251.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n251.data, hxc_l_symbol.data, hxc_l_tmp_class_field_load_result_n251.byte_length) == 0);
      }
      if (hxc_l_tmp_short_circuit_result_n69)
      {
        struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n255 = hxc_l_reader;
        if (hxc_l_tmp_load_result_n255 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n256 = hxc_l_idNode;
        hxc_l_gc_roots[79] = (const void *)hxc_l_tmp_load_result_n256;
        hxc_l_tmp_enum_payload_0_owner_n70 = (hxc_string){ (const uint8_t *)"messages", 8, true, NULL };
        if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n70) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_string hxc_l_tmp_enum_payload_0_owned_load_result_n257 = hxc_l_tmp_enum_payload_0_owner_n70;
        hxc_l_tmp_enum_payload_1_owner_n71 = hxc_l_id;
        if (hxc_string_retain(hxc_l_tmp_enum_payload_1_owner_n71) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_instance_call_argument_1_owner_n72 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaDuplicateId, .hxc_payload.hxc_SchemaDuplicateId.hxc_path = hxc_l_tmp_enum_payload_0_owned_load_result_n257, .hxc_payload.hxc_SchemaDuplicateId.hxc_id = hxc_l_tmp_enum_payload_1_owner_n71 };
        hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n255, hxc_l_tmp_load_result_n256, hxc_l_tmp_instance_call_argument_1_owner_n72);
        struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n263 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
        hxc_l_gc_roots[80] = hxc_l_tmp_call_result_n263.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n263.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
        hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n72);
        if (hxc_string_release(&hxc_l_symbol) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n63) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n61) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n57) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n55) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n54) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_array_ref_release(hxc_l_expectedSymbols) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_array_ref_release(hxc_l_expectedIds) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_defaultLocale) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_array_ref_release(hxc_l_locales) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_catalogId) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
        if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
        {
          abort();
        }
        return hxc_l_tmp_call_result_n263;
      }
    }
    hxc_l_gc_roots[77] = (const void *)hxc_l_messages;
    int32_t hxc_l_tmp_array_length_result_n265;
    if (hxc_array_ref_length(hxc_l_messages, &hxc_l_tmp_array_length_result_n265) != HXC_STATUS_OK)
    {
      abort();
    }
    bool hxc_l_tmp_short_circuit_result_n73 = hxc_l_tmp_array_length_result_n265 > 0;
    if (hxc_l_tmp_array_length_result_n265 > 0)
    {
      struct hxc_array_ref *hxc_l_tmp_load_result_n266 = hxc_l_messages;
      hxc_l_gc_roots[81] = (const void *)hxc_l_tmp_load_result_n266;
      hxc_l_gc_roots[82] = (const void *)hxc_l_messages;
      int32_t hxc_l_tmp_array_length_result_n268;
      if (hxc_array_ref_length(hxc_l_messages, &hxc_l_tmp_array_length_result_n268) != HXC_STATUS_OK)
      {
        abort();
      }
      struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *hxc_l_tmp_array_get_result_n269;
      if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n266, (size_t)hxc_i32_subtract_wrapping(hxc_l_tmp_array_length_result_n268, 1), &hxc_l_tmp_array_get_result_n269) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_gc_roots[83] = (const void *)hxc_l_tmp_array_get_result_n269;
      if (hxc_l_tmp_array_get_result_n269 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_class_field_load_result_n270 = (*hxc_l_tmp_array_get_result_n269).hxc_id;
      int32_t hxc_l_tmp_call_result_n272 = hxc_caxecraft_content_RuntimeSchemaReader_compareUtf8(hxc_l_tmp_class_field_load_result_n270, hxc_l_id);
      hxc_l_tmp_short_circuit_result_n73 = hxc_l_tmp_call_result_n272 > 0;
    }
    if (hxc_l_tmp_short_circuit_result_n73)
    {
      struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n274 = hxc_l_reader;
      if (hxc_l_tmp_load_result_n274 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n275 = hxc_l_idNode;
      hxc_l_gc_roots[84] = (const void *)hxc_l_tmp_load_result_n275;
      hxc_l_tmp_enum_payload_0_owner_n74 = (hxc_string){ (const uint8_t *)"messages", 8, true, NULL };
      if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n74) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_instance_call_argument_1_owner_n75 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaNonCanonicalOrder, .hxc_payload.hxc_SchemaNonCanonicalOrder.hxc_path = hxc_l_tmp_enum_payload_0_owner_n74 };
      hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n274, hxc_l_tmp_load_result_n275, hxc_l_tmp_instance_call_argument_1_owner_n75);
      struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n280 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
      hxc_l_gc_roots[85] = hxc_l_tmp_call_result_n280.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n280.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
      hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n75);
      if (hxc_string_release(&hxc_l_symbol) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n63) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n61) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n57) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n55) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n54) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_expectedSymbols) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_expectedIds) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_defaultLocale) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_locales) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_catalogId) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_tmp_call_result_n280;
    }
    int32_t hxc_l_tmp_load_result_n281 = hxc_l_index_h098f99b4ccb1;
    int32_t hxc_l_tmp_array_length_result_n283;
    if (hxc_array_ref_length(hxc_l_expectedIds, &hxc_l_tmp_array_length_result_n283) != HXC_STATUS_OK)
    {
      abort();
    }
    bool hxc_l_tmp_short_circuit_result_n76 = hxc_l_tmp_load_result_n281 < hxc_l_tmp_array_length_result_n283;
    if (hxc_l_tmp_load_result_n281 < hxc_l_tmp_array_length_result_n283)
    {
      hxc_string hxc_l_tmp_load_result_n284 = hxc_l_id;
      struct hxc_array_ref *hxc_l_tmp_load_result_n285 = hxc_l_expectedIds;
      hxc_string hxc_l_tmp_array_get_result_n287;
      if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n285, (size_t)hxc_l_index_h098f99b4ccb1, &hxc_l_tmp_array_get_result_n287) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_array_element_owner_n77 = hxc_l_tmp_array_get_result_n287;
      hxc_l_tmp_short_circuit_result_n76 = hxc_l_tmp_load_result_n284.data == NULL || hxc_l_tmp_array_element_owner_n77.data == NULL ? hxc_l_tmp_load_result_n284.data == hxc_l_tmp_array_element_owner_n77.data : hxc_l_tmp_load_result_n284.byte_length == hxc_l_tmp_array_element_owner_n77.byte_length && (hxc_l_tmp_load_result_n284.byte_length == 0 || memcmp(hxc_l_tmp_load_result_n284.data, hxc_l_tmp_array_element_owner_n77.data, hxc_l_tmp_load_result_n284.byte_length) == 0);
      if (hxc_string_release(&hxc_l_tmp_array_element_owner_n77) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    bool hxc_l_tmp_short_circuit_load_result_n290 = hxc_l_tmp_short_circuit_result_n76;
    bool hxc_l_tmp_short_circuit_result_n78 = hxc_l_tmp_short_circuit_load_result_n290;
    if (hxc_l_tmp_short_circuit_load_result_n290)
    {
      hxc_string hxc_l_tmp_load_result_n291 = hxc_l_symbol;
      struct hxc_array_ref *hxc_l_tmp_load_result_n292 = hxc_l_expectedSymbols;
      hxc_string hxc_l_tmp_array_get_result_n294;
      if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n292, (size_t)hxc_l_index_h098f99b4ccb1, &hxc_l_tmp_array_get_result_n294) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_array_element_owner_n79 = hxc_l_tmp_array_get_result_n294;
      hxc_l_tmp_short_circuit_result_n78 = !(hxc_l_tmp_load_result_n291.data == NULL || hxc_l_tmp_array_element_owner_n79.data == NULL ? hxc_l_tmp_load_result_n291.data == hxc_l_tmp_array_element_owner_n79.data : hxc_l_tmp_load_result_n291.byte_length == hxc_l_tmp_array_element_owner_n79.byte_length && (hxc_l_tmp_load_result_n291.byte_length == 0 || memcmp(hxc_l_tmp_load_result_n291.data, hxc_l_tmp_array_element_owner_n79.data, hxc_l_tmp_load_result_n291.byte_length) == 0));
      if (hxc_string_release(&hxc_l_tmp_array_element_owner_n79) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    if (hxc_l_tmp_short_circuit_result_n78)
    {
      struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n298 = hxc_l_reader;
      if (hxc_l_tmp_load_result_n298 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n299 = hxc_l_symbolNode;
      hxc_l_gc_roots[86] = (const void *)hxc_l_tmp_load_result_n299;
      hxc_l_tmp_enum_payload_0_owner_n80 = hxc_l_path;
      if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n80) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_instance_call_argument_1_owner_n81 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaIncompatibleTypedCatalog, .hxc_payload.hxc_SchemaIncompatibleTypedCatalog.hxc_path = hxc_l_tmp_enum_payload_0_owner_n80 };
      hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n298, hxc_l_tmp_load_result_n299, hxc_l_tmp_instance_call_argument_1_owner_n81);
      struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n305 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
      hxc_l_gc_roots[87] = hxc_l_tmp_call_result_n305.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n305.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
      hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n81);
      if (hxc_string_release(&hxc_l_symbol) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n63) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n61) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n57) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n55) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n54) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_expectedSymbols) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_expectedIds) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_defaultLocale) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_locales) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_catalogId) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_tmp_call_result_n305;
    }
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n306 = hxc_l_reader;
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n307 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n307 == NULL)
    {
      abort();
    }
    hxc_l_gc_roots[88] = (const void *)hxc_l_messageFields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n309 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n307, hxc_l_messageFields, (hxc_string){ (const uint8_t *)"text", 4, true, NULL });
    hxc_l_gc_roots[89] = (const void *)hxc_l_tmp_instance_call_result_n309;
    hxc_string hxc_l_tmp_string_concat_result_n311 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".text", 5, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n311) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n83 = hxc_l_tmp_string_concat_result_n311;
    hxc_string hxc_l_tmp_static_call_argument_2_borrow_result_n312 = hxc_l_tmp_static_call_argument_2_owner_n83;
    struct hxc_array_ref *hxc_l_tmp_call_result_n314 = hxc_caxecraft_localization_RuntimeUiCatalog_readTexts(hxc_l_tmp_load_result_n306, hxc_l_tmp_instance_call_result_n309, hxc_l_tmp_static_call_argument_2_borrow_result_n312, hxc_l_locales);
    hxc_l_texts = hxc_l_tmp_call_result_n314;
    if (hxc_l_texts == NULL)
    {
      struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n317 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
      hxc_l_gc_roots[90] = hxc_l_tmp_call_result_n317.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n317.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
      if (hxc_array_ref_release(hxc_l_texts) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n83) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_symbol) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n63) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n61) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n57) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n55) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n54) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_expectedSymbols) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_expectedIds) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_defaultLocale) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_locales) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_catalogId) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_tmp_call_result_n317;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n318 = hxc_l_messages;
    hxc_l_gc_roots[91] = (const void *)hxc_l_tmp_load_result_n318;
    hxc_string hxc_l_tmp_load_result_n319 = hxc_l_id;
    hxc_string hxc_l_tmp_load_result_n320 = hxc_l_symbol;
    struct hxc_array_ref *hxc_l_tmp_load_result_n321 = hxc_l_texts;
    struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *hxc_l_tmp_managed_class_result_n322 = NULL;
    if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition_descriptor, (void **)&hxc_l_tmp_managed_class_result_n322) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[92] = (const void *)hxc_l_tmp_managed_class_result_n322;
    hxc_compiler_constructor_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition(hxc_l_tmp_managed_class_result_n322, hxc_l_tmp_load_result_n319, hxc_l_tmp_load_result_n320, hxc_l_tmp_load_result_n321);
    hxc_l_gc_roots[93] = (const void *)hxc_l_tmp_managed_class_result_n322;
    int32_t hxc_l_tmp_array_push_result_n323;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n318, &(struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *){ hxc_l_tmp_managed_class_result_n322 }, &hxc_l_tmp_array_push_result_n323) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n323;
    if (hxc_array_ref_release(hxc_l_texts) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n83) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_symbol) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n63) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n61) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n57) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n55) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n54) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  hxc_l_gc_roots[60] = (const void *)hxc_l_messages;
  int32_t hxc_l_tmp_array_length_result_n325;
  if (hxc_array_ref_length(hxc_l_messages, &hxc_l_tmp_array_length_result_n325) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_tmp_array_length_result_n327;
  if (hxc_array_ref_length(hxc_l_expectedIds, &hxc_l_tmp_array_length_result_n327) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_tmp_array_length_result_n325 != hxc_l_tmp_array_length_result_n327)
  {
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n328 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n328 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n329 = hxc_l_messageNode;
    hxc_l_gc_roots[94] = (const void *)hxc_l_tmp_load_result_n329;
    hxc_l_tmp_enum_payload_0_owner_n84 = (hxc_string){ (const uint8_t *)"messages", 8, true, NULL };
    if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n84) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n85 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaIncompatibleTypedCatalog, .hxc_payload.hxc_SchemaIncompatibleTypedCatalog.hxc_path = hxc_l_tmp_enum_payload_0_owner_n84 };
    hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n328, hxc_l_tmp_load_result_n329, hxc_l_tmp_instance_call_argument_1_owner_n85);
    struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n334 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
    hxc_l_gc_roots[95] = hxc_l_tmp_call_result_n334.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n334.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
    hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n85);
    if (hxc_array_ref_release(hxc_l_expectedSymbols) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_expectedIds) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_defaultLocale) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_locales) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_catalogId) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n334;
  }
  int32_t hxc_l_g_h4c3d8b90894e = 0;
  hxc_l_gc_roots[96] = (const void *)hxc_l_messages;
  int32_t hxc_l_tmp_array_length_result_n336;
  if (hxc_array_ref_length(hxc_l_messages, &hxc_l_tmp_array_length_result_n336) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1_he5a08c16481e = hxc_l_tmp_array_length_result_n336;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n337 = hxc_l_g_h4c3d8b90894e;
    if (!(hxc_l_tmp_load_result_n337 < hxc_l_g1_he5a08c16481e))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n339 = hxc_l_g_h4c3d8b90894e;
    hxc_l_g_h4c3d8b90894e = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n339, 1);
    int32_t hxc_l_index_h694ddcecb4fd = hxc_l_tmp_increment_load_result_n339;
    struct hxc_array_ref *hxc_l_tmp_load_result_n340 = hxc_l_messages;
    hxc_l_gc_roots[97] = (const void *)hxc_l_tmp_load_result_n340;
    struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *hxc_l_tmp_array_get_result_n342;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n340, (size_t)hxc_l_index_h694ddcecb4fd, &hxc_l_tmp_array_get_result_n342) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[98] = (const void *)hxc_l_tmp_array_get_result_n342;
    if (hxc_l_tmp_array_get_result_n342 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n343 = (*hxc_l_tmp_array_get_result_n342).hxc_id;
    struct hxc_array_ref *hxc_l_tmp_load_result_n344 = hxc_l_expectedIds;
    hxc_string hxc_l_tmp_array_get_result_n346;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n344, (size_t)hxc_l_index_h694ddcecb4fd, &hxc_l_tmp_array_get_result_n346) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n89 = hxc_l_tmp_array_get_result_n346;
    hxc_string hxc_l_tmp_array_element_borrow_result_n347 = hxc_l_tmp_array_element_owner_n89;
    bool hxc_l_tmp_short_circuit_result_n90 = !(hxc_l_tmp_class_field_load_result_n343.data == NULL || hxc_l_tmp_array_element_borrow_result_n347.data == NULL ? hxc_l_tmp_class_field_load_result_n343.data == hxc_l_tmp_array_element_borrow_result_n347.data : hxc_l_tmp_class_field_load_result_n343.byte_length == hxc_l_tmp_array_element_borrow_result_n347.byte_length && (hxc_l_tmp_class_field_load_result_n343.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n343.data, hxc_l_tmp_array_element_borrow_result_n347.data, hxc_l_tmp_class_field_load_result_n343.byte_length) == 0));
    if (!!(hxc_l_tmp_class_field_load_result_n343.data == NULL || hxc_l_tmp_array_element_borrow_result_n347.data == NULL ? hxc_l_tmp_class_field_load_result_n343.data == hxc_l_tmp_array_element_borrow_result_n347.data : hxc_l_tmp_class_field_load_result_n343.byte_length == hxc_l_tmp_array_element_borrow_result_n347.byte_length && (hxc_l_tmp_class_field_load_result_n343.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n343.data, hxc_l_tmp_array_element_borrow_result_n347.data, hxc_l_tmp_class_field_load_result_n343.byte_length) == 0)))
    {
      struct hxc_array_ref *hxc_l_tmp_load_result_n349 = hxc_l_messages;
      hxc_l_gc_roots[103] = (const void *)hxc_l_tmp_load_result_n349;
      struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *hxc_l_tmp_array_get_result_n351;
      if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n349, (size_t)hxc_l_index_h694ddcecb4fd, &hxc_l_tmp_array_get_result_n351) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_gc_roots[104] = (const void *)hxc_l_tmp_array_get_result_n351;
      if (hxc_l_tmp_array_get_result_n351 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_class_field_load_result_n352 = (*hxc_l_tmp_array_get_result_n351).hxc_symbol;
      struct hxc_array_ref *hxc_l_tmp_load_result_n353 = hxc_l_expectedSymbols;
      hxc_string hxc_l_tmp_array_get_result_n355;
      if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n353, (size_t)hxc_l_index_h694ddcecb4fd, &hxc_l_tmp_array_get_result_n355) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_array_element_owner_n91 = hxc_l_tmp_array_get_result_n355;
      hxc_l_tmp_short_circuit_result_n90 = !(hxc_l_tmp_class_field_load_result_n352.data == NULL || hxc_l_tmp_array_element_owner_n91.data == NULL ? hxc_l_tmp_class_field_load_result_n352.data == hxc_l_tmp_array_element_owner_n91.data : hxc_l_tmp_class_field_load_result_n352.byte_length == hxc_l_tmp_array_element_owner_n91.byte_length && (hxc_l_tmp_class_field_load_result_n352.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n352.data, hxc_l_tmp_array_element_owner_n91.data, hxc_l_tmp_class_field_load_result_n352.byte_length) == 0));
      if (hxc_string_release(&hxc_l_tmp_array_element_owner_n91) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    bool hxc_l_tmp_short_circuit_load_result_n358 = hxc_l_tmp_short_circuit_result_n90;
    if (hxc_string_release(&hxc_l_tmp_array_element_owner_n89) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_l_tmp_short_circuit_load_result_n358)
    {
      struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n359 = hxc_l_reader;
      if (hxc_l_tmp_load_result_n359 == NULL)
      {
        abort();
      }
      struct hxc_array_ref *hxc_l_tmp_load_result_n360 = hxc_l_messageValues;
      hxc_l_gc_roots[105] = (const void *)hxc_l_tmp_load_result_n360;
      struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_array_get_result_n362;
      if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n360, (size_t)hxc_l_index_h694ddcecb4fd, &hxc_l_tmp_array_get_result_n362) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_gc_roots[106] = (const void *)hxc_l_tmp_array_get_result_n362;
      hxc_string hxc_l_tmp_std_string_int_result_n364 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_from_int32(hxc_l_index_h694ddcecb4fd, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n364) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_string_concat_right_owner_n92 = hxc_l_tmp_std_string_int_result_n364;
      hxc_string hxc_l_tmp_string_concat_result_n366 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"messages[", 9, true, NULL }, hxc_l_tmp_string_concat_right_owner_n92, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n366) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_string_concat_left_owner_n93 = hxc_l_tmp_string_concat_result_n366;
      hxc_string hxc_l_tmp_string_concat_result_n368 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n93, (hxc_string){ (const uint8_t *)"]", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n368) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_instance_call_argument_1_owner_n94 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaIncompatibleTypedCatalog, .hxc_payload.hxc_SchemaIncompatibleTypedCatalog.hxc_path = hxc_l_tmp_string_concat_result_n368 };
      hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n359, hxc_l_tmp_array_get_result_n362, hxc_l_tmp_instance_call_argument_1_owner_n94);
      struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n372 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
      hxc_l_gc_roots[107] = hxc_l_tmp_call_result_n372.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n372.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
      hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n94);
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n93) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n92) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_expectedSymbols) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_expectedIds) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_defaultLocale) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_locales) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_catalogId) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_tmp_call_result_n372;
    }
  }
  hxc_string hxc_l_tmp_load_result_n373 = hxc_l_catalogId;
  struct hxc_array_ref *hxc_l_tmp_load_result_n374 = hxc_l_locales;
  struct hxc_array_ref *hxc_l_tmp_load_result_n375 = hxc_l_messages;
  hxc_l_gc_roots[99] = (const void *)hxc_l_tmp_load_result_n375;
  struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_managed_class_result_n376 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_localization_RuntimeUiCatalog_descriptor, (void **)&hxc_l_tmp_managed_class_result_n376) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[100] = (const void *)hxc_l_tmp_managed_class_result_n376;
  hxc_compiler_constructor_caxecraft_localization_RuntimeUiCatalog(hxc_l_tmp_managed_class_result_n376, hxc_l_tmp_load_result_n373, hxc_l_tmp_load_result_n374, hxc_l_tmp_load_result_n375);
  hxc_l_gc_roots[101] = (const void *)hxc_l_tmp_managed_class_result_n376;
  hxc_l_gc_roots[102] = (struct hxc_caxecraft_localization_RuntimeUiCatalogResult){ .hxc_tag = hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady, .hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog = hxc_l_tmp_managed_class_result_n376 }.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)(struct hxc_caxecraft_localization_RuntimeUiCatalogResult){ .hxc_tag = hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady, .hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog = hxc_l_tmp_managed_class_result_n376 }.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
  if (hxc_array_ref_release(hxc_l_expectedSymbols) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_expectedIds) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_defaultLocale) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_locales) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_catalogId) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_localization_RuntimeUiCatalogResult){ .hxc_tag = hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady, .hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog = hxc_l_tmp_managed_class_result_n376 };
}

struct hxc_array_ref *hxc_caxecraft_localization_RuntimeUiCatalog_expectedMessageIds(void)
{
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n0 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"aquatic_gear_equipped", 21, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"brand", 5, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"capture_prompt", 14, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"controls", 8, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"debug_cells", 11, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"debug_draws", 11, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"debug_frame", 11, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"debug_tick", 10, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"debug_visible", 13, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"editor_advanced", 15, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"editor_back", 11, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"editor_canvas_help", 18, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"editor_invalid", 14, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"editor_name", 11, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"editor_new_world", 16, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"editor_ready", 12, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"editor_redo", 11, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"editor_stop_test", 16, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"editor_test", 11, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"editor_testing", 14, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"editor_title", 12, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"editor_tool_list", 16, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"editor_undo", 11, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"editor_valid", 12, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"editor_validate", 15, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"health_full", 11, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"menu_adventure", 14, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"menu_creative", 13, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"menu_editor", 11, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"menu_instructions", 17, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"no_block_in_reach", 17, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"pause_help", 10, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"pause_title", 11, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"place_blocked", 13, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"title_fallback", 14, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_array_create_result_n0;
}

struct hxc_array_ref *hxc_caxecraft_localization_RuntimeUiCatalog_expectedMessageSymbols(void)
{
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n0 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"AquaticGearEquipped", 19, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"Brand", 5, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"CapturePrompt", 13, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"Controls", 8, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"DebugCells", 10, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"DebugDraws", 10, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"DebugFrame", 10, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"DebugTick", 9, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"DebugVisible", 12, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"EditorAdvanced", 14, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"EditorBack", 10, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"EditorCanvasHelp", 16, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"EditorInvalid", 13, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"EditorName", 10, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"EditorNewWorld", 14, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"EditorReady", 11, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"EditorRedo", 10, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"EditorStopTest", 14, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"EditorTest", 10, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"EditorTesting", 13, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"EditorTitle", 11, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"EditorToolList", 14, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"EditorUndo", 10, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"EditorValid", 11, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"EditorValidate", 14, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"HealthFull", 10, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"MenuAdventure", 13, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"MenuCreative", 12, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"MenuEditor", 10, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"MenuInstructions", 16, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"NoBlockInReach", 14, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"PauseHelp", 9, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"PauseTitle", 10, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"PlaceBlocked", 12, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n0->value, &(hxc_string){ (const uint8_t *)"TitleFallback", 13, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_array_create_result_n0;
}

int32_t hxc_caxecraft_localization_RuntimeUiCatalog_localeStorageCode(int32_t hxc_l_locale)
{
  switch (hxc_l_locale) {
    case 0:
      {
        break;
      }
    case 1:
      {
        return 1;
      }
    default:
      {
        return -1;
      }
  }
  return 0;
}

int32_t hxc_caxecraft_localization_RuntimeUiCatalog_messageStorageCode(int32_t hxc_l_message)
{
  switch (hxc_l_message) {
    case 0:
      {
        break;
      }
    case 1:
      {
        return 1;
      }
    case 2:
      {
        return 2;
      }
    case 3:
      {
        return 3;
      }
    case 4:
      {
        return 4;
      }
    case 5:
      {
        return 5;
      }
    case 6:
      {
        return 6;
      }
    case 7:
      {
        return 7;
      }
    case 8:
      {
        return 8;
      }
    case 9:
      {
        return 9;
      }
    case 10:
      {
        return 10;
      }
    case 11:
      {
        return 11;
      }
    case 12:
      {
        return 12;
      }
    case 13:
      {
        return 13;
      }
    case 14:
      {
        return 14;
      }
    case 15:
      {
        return 15;
      }
    case 16:
      {
        return 16;
      }
    case 17:
      {
        return 17;
      }
    case 18:
      {
        return 18;
      }
    case 19:
      {
        return 19;
      }
    case 20:
      {
        return 20;
      }
    case 21:
      {
        return 21;
      }
    case 22:
      {
        return 22;
      }
    case 23:
      {
        return 23;
      }
    case 24:
      {
        return 24;
      }
    case 25:
      {
        return 25;
      }
    case 26:
      {
        return 26;
      }
    case 27:
      {
        return 27;
      }
    case 28:
      {
        return 28;
      }
    case 29:
      {
        return 29;
      }
    case 30:
      {
        return 30;
      }
    case 31:
      {
        return 31;
      }
    case 32:
      {
        return 32;
      }
    case 33:
      {
        return 33;
      }
    case 34:
      {
        return 34;
      }
    default:
      {
        return -1;
      }
  }
  return 0;
}

hxc_string hxc_caxecraft_localization_RuntimeUiCatalog_readText(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, struct hxc_caxecraft_content_ContentJsonNode *hxc_l_node, hxc_string hxc_l_path)
{
  const void *hxc_l_gc_roots[7] = { (const void *)hxc_l_node, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 7, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_managed_flow_owner_n10 = { 0 };
  struct hxc_bytes_ref *hxc_l_tmp_bytes_length_receiver_owner_n12 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n15 = { 0 };
  hxc_string hxc_l_text = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n6 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_1_owner_n7 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n8 = { 0 };
  hxc_string hxc_l_tmp_returned_string_owner_n9 = { 0 };
  hxc_string hxc_l_value = { 0 };
  if (hxc_l_node == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_ContentJsonValue hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_node).hxc_value;
  hxc_l_gc_roots[1] = hxc_l_tmp_class_field_load_result_n0.hxc_tag == hxc_caxecraft_content_ContentJsonValue_JsonArray ? (const void *)hxc_l_tmp_class_field_load_result_n0.hxc_payload.hxc_JsonArray.hxc_values : NULL;
  hxc_l_gc_roots[2] = hxc_l_tmp_class_field_load_result_n0.hxc_tag == hxc_caxecraft_content_ContentJsonValue_JsonObject ? (const void *)hxc_l_tmp_class_field_load_result_n0.hxc_payload.hxc_JsonObject.hxc_fields : NULL;
  struct hxc_caxecraft_content_ContentJsonValue hxc_l_g = hxc_l_tmp_class_field_load_result_n0;
  if (hxc_enum_3a7ff4b0_retain(&hxc_l_g) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[3] = hxc_l_g.hxc_tag == hxc_caxecraft_content_ContentJsonValue_JsonArray ? (const void *)hxc_l_g.hxc_payload.hxc_JsonArray.hxc_values : NULL;
  hxc_l_gc_roots[4] = hxc_l_g.hxc_tag == hxc_caxecraft_content_ContentJsonValue_JsonObject ? (const void *)hxc_l_g.hxc_payload.hxc_JsonObject.hxc_fields : NULL;
  if (!(hxc_l_g.hxc_tag == hxc_caxecraft_content_ContentJsonValue_JsonString))
  {
    if (hxc_l_reader == NULL)
    {
      abort();
    }
    hxc_l_tmp_enum_payload_0_owner_n6 = hxc_l_path;
    if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_string hxc_l_tmp_enum_payload_0_owned_load_result_n5 = hxc_l_tmp_enum_payload_0_owner_n6;
    hxc_l_tmp_enum_payload_1_owner_n7 = (hxc_string){ (const uint8_t *)"display string", 14, true, NULL };
    if (hxc_string_retain(hxc_l_tmp_enum_payload_1_owner_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n8 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaWrongType, .hxc_payload.hxc_SchemaWrongType.hxc_path = hxc_l_tmp_enum_payload_0_owned_load_result_n5, .hxc_payload.hxc_SchemaWrongType.hxc_expected = hxc_l_tmp_enum_payload_1_owner_n7 };
    hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_reader, hxc_l_node, hxc_l_tmp_instance_call_argument_1_owner_n8);
    hxc_l_tmp_returned_string_owner_n9 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_string hxc_l_tmp_returned_string_owned_load_result_n9 = hxc_l_tmp_returned_string_owner_n9;
    hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n8);
    hxc_enum_3a7ff4b0_destroy(&hxc_l_g);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_returned_string_owned_load_result_n9;
  }
  hxc_l_gc_roots[5] = hxc_l_g.hxc_tag == hxc_caxecraft_content_ContentJsonValue_JsonArray ? (const void *)hxc_l_g.hxc_payload.hxc_JsonArray.hxc_values : NULL;
  hxc_l_gc_roots[6] = hxc_l_g.hxc_tag == hxc_caxecraft_content_ContentJsonValue_JsonObject ? (const void *)hxc_l_g.hxc_payload.hxc_JsonObject.hxc_fields : NULL;
  if (hxc_l_g.hxc_tag != hxc_caxecraft_content_ContentJsonValue_JsonString)
  {
    abort();
  }
  hxc_string hxc_l_tmp_enum_payload_project_n3 = hxc_l_g.hxc_payload.hxc_JsonString.hxc_value;
  hxc_l_text = hxc_l_tmp_enum_payload_project_n3;
  if (hxc_string_retain(hxc_l_text) != HXC_STATUS_OK)
  {
    abort();
  }
  (void)hxc_l_text;
  hxc_l_value = hxc_l_text;
  if (hxc_string_retain(hxc_l_value) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_text) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_managed_flow_carrier_move_result_n10 = hxc_l_value;
  hxc_l_tmp_managed_flow_owner_n10 = hxc_l_tmp_managed_flow_carrier_move_result_n10;
  hxc_string hxc_l_tmp_managed_flow_owner_load_result_n11 = hxc_l_tmp_managed_flow_owner_n10;
  if (hxc_l_tmp_managed_flow_owner_load_result_n11.data == NULL)
  {
    abort();
  }
  int32_t hxc_l_tmp_string_length_result_n12;
  if (hxc_string_haxe_length(hxc_l_tmp_managed_flow_owner_load_result_n11, &hxc_l_tmp_string_length_result_n12) != HXC_STATUS_OK)
  {
    abort();
  }
  bool hxc_l_tmp_short_circuit_result_n11 = hxc_l_tmp_string_length_result_n12 == 0;
  if (!(hxc_l_tmp_string_length_result_n12 == 0))
  {
    hxc_string hxc_l_tmp_managed_flow_owner_load_result_n13 = hxc_l_tmp_managed_flow_owner_n10;
    if (hxc_l_tmp_managed_flow_owner_load_result_n13.data == NULL)
    {
      abort();
    }
    struct hxc_bytes_ref *hxc_l_tmp_bytes_of_string_utf8_result_n14 = NULL;
    if (hxc_bytes_ref_create_utf8_copy(hxc_default_allocator(), hxc_l_tmp_managed_flow_owner_load_result_n13, &hxc_l_tmp_bytes_of_string_utf8_result_n14) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_bytes_length_receiver_owner_n12 = hxc_l_tmp_bytes_of_string_utf8_result_n14;
    int32_t hxc_l_tmp_bytes_length_result_n16;
    if (hxc_bytes_ref_length(hxc_l_tmp_bytes_length_receiver_owner_n12, &hxc_l_tmp_bytes_length_result_n16) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n11 = hxc_l_tmp_bytes_length_result_n16 > 240;
    if (hxc_bytes_ref_release(hxc_l_tmp_bytes_length_receiver_owner_n12) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  bool hxc_l_tmp_short_circuit_load_result_n17 = hxc_l_tmp_short_circuit_result_n11;
  bool hxc_l_tmp_short_circuit_result_n13 = hxc_l_tmp_short_circuit_load_result_n17;
  if (!hxc_l_tmp_short_circuit_load_result_n17)
  {
    bool hxc_l_tmp_call_result_n19 = hxc_caxecraft_content_RuntimeSchemaReader_hasControl(hxc_l_tmp_managed_flow_owner_n10);
    hxc_l_tmp_short_circuit_result_n13 = hxc_l_tmp_call_result_n19;
  }
  if (!hxc_l_tmp_short_circuit_result_n13)
  {
    hxc_string hxc_l_tmp_managed_flow_owner_load_result_n25 = hxc_l_tmp_managed_flow_owner_n10;
    hxc_enum_3a7ff4b0_destroy(&hxc_l_g);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_managed_flow_owner_load_result_n25;
  }
  if (hxc_l_reader == NULL)
  {
    abort();
  }
  hxc_string hxc_l_tmp_enum_payload_0_owner_n14 = hxc_l_path;
  if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n14) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_instance_call_argument_1_owner_n15 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidText, .hxc_payload.hxc_SchemaInvalidText.hxc_path = hxc_l_tmp_enum_payload_0_owner_n14 };
  hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_reader, hxc_l_node, hxc_l_tmp_instance_call_argument_1_owner_n15);
  hxc_string hxc_l_tmp_returned_string_owner_n16 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n16) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_returned_string_owned_load_result_n24 = hxc_l_tmp_returned_string_owner_n16;
  hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n15);
  if (hxc_string_release(&hxc_l_tmp_managed_flow_owner_n10) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_enum_3a7ff4b0_destroy(&hxc_l_g);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_returned_string_owned_load_result_n24;
}

struct hxc_array_ref *hxc_caxecraft_localization_RuntimeUiCatalog_readTextFields(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, struct hxc_caxecraft_content_ContentJsonNode *hxc_l_node, hxc_string hxc_l_path, struct hxc_array_ref *hxc_l_locales, struct hxc_array_ref *hxc_l_fields)
{
  const void *hxc_l_gc_roots[15] = { (const void *)hxc_l_node, (const void *)hxc_l_fields, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 15, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_locale_hab6a4c8bb9fd = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n12 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n13 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_2_owner_n14 = { 0 };
  struct hxc_array_ref *hxc_l_result = { 0 };
  hxc_string hxc_l_locale_h7a514f4fcc15 = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n18 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n22 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n23 = { 0 };
  hxc_string hxc_l_text = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n25 = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n26 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n5 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n6 = { 0 };
  int32_t hxc_l_tmp_array_length_result_n0;
  if (hxc_array_ref_length(hxc_l_fields, &hxc_l_tmp_array_length_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_tmp_array_length_result_n1;
  if (hxc_array_ref_length(hxc_l_locales, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_tmp_array_length_result_n0 != hxc_l_tmp_array_length_result_n1)
  {
    if (hxc_l_reader == NULL)
    {
      abort();
    }
    hxc_l_tmp_enum_payload_0_owner_n5 = hxc_l_path;
    if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n6 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidLocale, .hxc_payload.hxc_SchemaInvalidLocale.hxc_path = hxc_l_tmp_enum_payload_0_owner_n5 };
    hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_reader, hxc_l_node, hxc_l_tmp_instance_call_argument_1_owner_n6);
    hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n6);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  int32_t hxc_l_g_h00c64b999956 = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n5 = hxc_l_g_h00c64b999956;
    int32_t hxc_l_tmp_array_length_result_n6;
    if (hxc_array_ref_length(hxc_l_fields, &hxc_l_tmp_array_length_result_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n5 < hxc_l_tmp_array_length_result_n6))
    {
      break;
    }
    struct hxc_caxecraft_content_ContentJsonField *hxc_l_tmp_array_get_result_n8;
    if (hxc_array_ref_get_copy(hxc_l_fields, (size_t)hxc_l_g_h00c64b999956, &hxc_l_tmp_array_get_result_n8) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_array_get_result_n8;
    struct hxc_caxecraft_content_ContentJsonField *hxc_l_field_h26b10096402b = hxc_l_tmp_array_get_result_n8;
    hxc_l_g_h00c64b999956 = hxc_i32_add_wrapping(hxc_l_g_h00c64b999956, 1);
    bool hxc_l_admitted = false;
    int32_t hxc_l_g_he77cc982d8a5 = 0;
    while (1)
    {
      int32_t hxc_l_tmp_load_result_n10 = hxc_l_g_he77cc982d8a5;
      int32_t hxc_l_tmp_array_length_result_n11;
      if (hxc_array_ref_length(hxc_l_locales, &hxc_l_tmp_array_length_result_n11) != HXC_STATUS_OK)
      {
        abort();
      }
      if (!(hxc_l_tmp_load_result_n10 < hxc_l_tmp_array_length_result_n11))
      {
        break;
      }
      hxc_string hxc_l_tmp_array_get_result_n13;
      if (hxc_array_ref_get_copy(hxc_l_locales, (size_t)hxc_l_g_he77cc982d8a5, &hxc_l_tmp_array_get_result_n13) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_array_element_owner_n12 = hxc_l_tmp_array_get_result_n13;
      hxc_l_locale_hab6a4c8bb9fd = hxc_l_tmp_array_element_owner_n12;
      if (hxc_string_retain(hxc_l_locale_hab6a4c8bb9fd) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_g_he77cc982d8a5 = hxc_i32_add_wrapping(hxc_l_g_he77cc982d8a5, 1);
      struct hxc_caxecraft_content_ContentJsonField *hxc_l_tmp_load_result_n16 = hxc_l_field_h26b10096402b;
      hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_load_result_n16;
      if (hxc_l_tmp_load_result_n16 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_class_field_load_result_n17 = (*hxc_l_tmp_load_result_n16).hxc_name;
      if (hxc_l_tmp_class_field_load_result_n17.data == NULL || hxc_l_locale_hab6a4c8bb9fd.data == NULL ? hxc_l_tmp_class_field_load_result_n17.data == hxc_l_locale_hab6a4c8bb9fd.data : hxc_l_tmp_class_field_load_result_n17.byte_length == hxc_l_locale_hab6a4c8bb9fd.byte_length && (hxc_l_tmp_class_field_load_result_n17.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n17.data, hxc_l_locale_hab6a4c8bb9fd.data, hxc_l_tmp_class_field_load_result_n17.byte_length) == 0))
      {
        hxc_l_admitted = true;
      }
      if (hxc_string_release(&hxc_l_locale_hab6a4c8bb9fd) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_array_element_owner_n12) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    if (!hxc_l_admitted)
    {
      if (hxc_l_reader == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_ContentJsonField *hxc_l_tmp_load_result_n21 = hxc_l_field_h26b10096402b;
      hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n21;
      if (hxc_l_tmp_load_result_n21 == NULL)
      {
        abort();
      }
      int32_t hxc_l_tmp_class_field_load_result_n22 = (*hxc_l_tmp_load_result_n21).hxc_line;
      struct hxc_caxecraft_content_ContentJsonField *hxc_l_tmp_load_result_n23 = hxc_l_field_h26b10096402b;
      hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_load_result_n23;
      if (hxc_l_tmp_load_result_n23 == NULL)
      {
        abort();
      }
      int32_t hxc_l_tmp_class_field_load_result_n24 = (*hxc_l_tmp_load_result_n23).hxc_column;
      hxc_string hxc_l_tmp_string_concat_result_n25 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n25) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_string_concat_left_owner_n13 = hxc_l_tmp_string_concat_result_n25;
      hxc_string hxc_l_tmp_string_concat_left_borrow_result_n26 = hxc_l_tmp_string_concat_left_owner_n13;
      struct hxc_caxecraft_content_ContentJsonField *hxc_l_tmp_load_result_n27 = hxc_l_field_h26b10096402b;
      hxc_l_gc_roots[6] = (const void *)hxc_l_tmp_load_result_n27;
      if (hxc_l_tmp_load_result_n27 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_class_field_load_result_n28 = (*hxc_l_tmp_load_result_n27).hxc_name;
      hxc_string hxc_l_tmp_string_concat_result_n29 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_borrow_result_n26, hxc_l_tmp_class_field_load_result_n28, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n29) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_instance_call_argument_2_owner_n14 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidLocale, .hxc_payload.hxc_SchemaInvalidLocale.hxc_path = hxc_l_tmp_string_concat_result_n29 };
      hxc_caxecraft_content_RuntimeSchemaReader_rejectAt(hxc_l_reader, hxc_l_tmp_class_field_load_result_n22, hxc_l_tmp_class_field_load_result_n24, hxc_l_tmp_instance_call_argument_2_owner_n14);
      hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_2_owner_n14);
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n13) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
  }
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n32 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n32) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_result = hxc_l_tmp_array_create_result_n32;
  int32_t hxc_l_g_h1941bc50a2fa = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n33 = hxc_l_g_h1941bc50a2fa;
    int32_t hxc_l_tmp_array_length_result_n34;
    if (hxc_array_ref_length(hxc_l_locales, &hxc_l_tmp_array_length_result_n34) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n33 < hxc_l_tmp_array_length_result_n34))
    {
      break;
    }
    hxc_string hxc_l_tmp_array_get_result_n36;
    if (hxc_array_ref_get_copy(hxc_l_locales, (size_t)hxc_l_g_h1941bc50a2fa, &hxc_l_tmp_array_get_result_n36) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n18 = hxc_l_tmp_array_get_result_n36;
    hxc_l_locale_h7a514f4fcc15 = hxc_l_tmp_array_element_owner_n18;
    if (hxc_string_retain(hxc_l_locale_h7a514f4fcc15) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g_h1941bc50a2fa = hxc_i32_add_wrapping(hxc_l_g_h1941bc50a2fa, 1);
    hxc_l_gc_roots[7] = (const void *)NULL;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_found = NULL;
    int32_t hxc_l_g_hf7c6c130178c = 0;
    while (1)
    {
      int32_t hxc_l_tmp_load_result_n39 = hxc_l_g_hf7c6c130178c;
      int32_t hxc_l_tmp_array_length_result_n40;
      if (hxc_array_ref_length(hxc_l_fields, &hxc_l_tmp_array_length_result_n40) != HXC_STATUS_OK)
      {
        abort();
      }
      if (!(hxc_l_tmp_load_result_n39 < hxc_l_tmp_array_length_result_n40))
      {
        break;
      }
      struct hxc_caxecraft_content_ContentJsonField *hxc_l_tmp_array_get_result_n42;
      if (hxc_array_ref_get_copy(hxc_l_fields, (size_t)hxc_l_g_hf7c6c130178c, &hxc_l_tmp_array_get_result_n42) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_gc_roots[8] = (const void *)hxc_l_tmp_array_get_result_n42;
      struct hxc_caxecraft_content_ContentJsonField *hxc_l_field_h26d1bfa1d97f = hxc_l_tmp_array_get_result_n42;
      hxc_l_g_hf7c6c130178c = hxc_i32_add_wrapping(hxc_l_g_hf7c6c130178c, 1);
      struct hxc_caxecraft_content_ContentJsonField *hxc_l_tmp_load_result_n44 = hxc_l_field_h26d1bfa1d97f;
      hxc_l_gc_roots[9] = (const void *)hxc_l_tmp_load_result_n44;
      if (hxc_l_tmp_load_result_n44 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_class_field_load_result_n45 = (*hxc_l_tmp_load_result_n44).hxc_name;
      if (hxc_l_tmp_class_field_load_result_n45.data == NULL || hxc_l_locale_h7a514f4fcc15.data == NULL ? hxc_l_tmp_class_field_load_result_n45.data == hxc_l_locale_h7a514f4fcc15.data : hxc_l_tmp_class_field_load_result_n45.byte_length == hxc_l_locale_h7a514f4fcc15.byte_length && (hxc_l_tmp_class_field_load_result_n45.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n45.data, hxc_l_locale_h7a514f4fcc15.data, hxc_l_tmp_class_field_load_result_n45.byte_length) == 0))
      {
        struct hxc_caxecraft_content_ContentJsonField *hxc_l_tmp_load_result_n48 = hxc_l_field_h26d1bfa1d97f;
        hxc_l_gc_roots[12] = (const void *)hxc_l_tmp_load_result_n48;
        if (hxc_l_tmp_load_result_n48 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_class_field_load_result_n49 = (*hxc_l_tmp_load_result_n48).hxc_value;
        hxc_l_gc_roots[13] = (const void *)hxc_l_tmp_class_field_load_result_n49;
        hxc_l_found = hxc_l_tmp_class_field_load_result_n49;
      }
    }
    hxc_l_gc_roots[10] = (const void *)hxc_l_found;
    hxc_l_gc_roots[11] = (const void *)NULL;
    if (hxc_l_found == NULL)
    {
      if (hxc_l_reader == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_string_concat_result_n51 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n51) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_string_concat_left_owner_n22 = hxc_l_tmp_string_concat_result_n51;
      hxc_string hxc_l_tmp_string_concat_left_borrow_result_n52 = hxc_l_tmp_string_concat_left_owner_n22;
      hxc_string hxc_l_tmp_string_concat_result_n54 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_borrow_result_n52, hxc_l_locale_h7a514f4fcc15, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n54) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_instance_call_argument_1_owner_n23 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidLocale, .hxc_payload.hxc_SchemaInvalidLocale.hxc_path = hxc_l_tmp_string_concat_result_n54 };
      hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_reader, hxc_l_node, hxc_l_tmp_instance_call_argument_1_owner_n23);
      hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n23);
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n22) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_locale_h7a514f4fcc15) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_array_element_owner_n18) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_result) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n57 = hxc_l_found;
    hxc_l_gc_roots[14] = (const void *)hxc_l_tmp_load_result_n57;
    hxc_string hxc_l_tmp_string_concat_result_n58 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n58) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_left_owner_n25 = hxc_l_tmp_string_concat_result_n58;
    hxc_string hxc_l_tmp_string_concat_left_borrow_result_n59 = hxc_l_tmp_string_concat_left_owner_n25;
    hxc_string hxc_l_tmp_string_concat_result_n61 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_borrow_result_n59, hxc_l_locale_h7a514f4fcc15, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n61) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n26 = hxc_l_tmp_string_concat_result_n61;
    hxc_string hxc_l_tmp_call_result_n63 = hxc_caxecraft_localization_RuntimeUiCatalog_readText(hxc_l_reader, hxc_l_tmp_load_result_n57, hxc_l_tmp_static_call_argument_2_owner_n26);
    hxc_l_text = hxc_l_tmp_call_result_n63;
    if (hxc_l_text.data == NULL)
    {
      if (hxc_string_release(&hxc_l_text) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n26) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n25) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_locale_h7a514f4fcc15) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_array_element_owner_n18) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_result) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n65 = hxc_l_result;
    int32_t hxc_l_tmp_array_push_result_n67;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n65, &hxc_l_text, &hxc_l_tmp_array_push_result_n67) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n67;
    if (hxc_string_release(&hxc_l_text) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n26) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n25) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_locale_h7a514f4fcc15) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_array_element_owner_n18) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_result;
}

struct hxc_array_ref *hxc_caxecraft_localization_RuntimeUiCatalog_readTexts(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, struct hxc_caxecraft_content_ContentJsonNode *hxc_l_node, hxc_string hxc_l_path, struct hxc_array_ref *hxc_l_locales)
{
  const void *hxc_l_gc_roots[9] = { (const void *)hxc_l_node, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 9, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_enum_payload_0_owner_n6 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_1_owner_n7 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n8 = { 0 };
  if (hxc_l_node == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_content_ContentJsonValue hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_node).hxc_value;
  hxc_l_gc_roots[1] = hxc_l_tmp_class_field_load_result_n0.hxc_tag == hxc_caxecraft_content_ContentJsonValue_JsonArray ? (const void *)hxc_l_tmp_class_field_load_result_n0.hxc_payload.hxc_JsonArray.hxc_values : NULL;
  hxc_l_gc_roots[2] = hxc_l_tmp_class_field_load_result_n0.hxc_tag == hxc_caxecraft_content_ContentJsonValue_JsonObject ? (const void *)hxc_l_tmp_class_field_load_result_n0.hxc_payload.hxc_JsonObject.hxc_fields : NULL;
  struct hxc_caxecraft_content_ContentJsonValue hxc_l_g = hxc_l_tmp_class_field_load_result_n0;
  if (hxc_enum_3a7ff4b0_retain(&hxc_l_g) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[3] = hxc_l_g.hxc_tag == hxc_caxecraft_content_ContentJsonValue_JsonArray ? (const void *)hxc_l_g.hxc_payload.hxc_JsonArray.hxc_values : NULL;
  hxc_l_gc_roots[4] = hxc_l_g.hxc_tag == hxc_caxecraft_content_ContentJsonValue_JsonObject ? (const void *)hxc_l_g.hxc_payload.hxc_JsonObject.hxc_fields : NULL;
  if (!(hxc_l_g.hxc_tag == hxc_caxecraft_content_ContentJsonValue_JsonObject))
  {
    if (hxc_l_reader == NULL)
    {
      abort();
    }
    hxc_l_tmp_enum_payload_0_owner_n6 = hxc_l_path;
    if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_string hxc_l_tmp_enum_payload_0_owned_load_result_n6 = hxc_l_tmp_enum_payload_0_owner_n6;
    hxc_l_tmp_enum_payload_1_owner_n7 = (hxc_string){ (const uint8_t *)"locale text object", 18, true, NULL };
    if (hxc_string_retain(hxc_l_tmp_enum_payload_1_owner_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n8 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaWrongType, .hxc_payload.hxc_SchemaWrongType.hxc_path = hxc_l_tmp_enum_payload_0_owned_load_result_n6, .hxc_payload.hxc_SchemaWrongType.hxc_expected = hxc_l_tmp_enum_payload_1_owner_n7 };
    hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_reader, hxc_l_node, hxc_l_tmp_instance_call_argument_1_owner_n8);
    hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n8);
    hxc_enum_3a7ff4b0_destroy(&hxc_l_g);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  hxc_l_gc_roots[5] = hxc_l_g.hxc_tag == hxc_caxecraft_content_ContentJsonValue_JsonArray ? (const void *)hxc_l_g.hxc_payload.hxc_JsonArray.hxc_values : NULL;
  hxc_l_gc_roots[6] = hxc_l_g.hxc_tag == hxc_caxecraft_content_ContentJsonValue_JsonObject ? (const void *)hxc_l_g.hxc_payload.hxc_JsonObject.hxc_fields : NULL;
  if (hxc_l_g.hxc_tag != hxc_caxecraft_content_ContentJsonValue_JsonObject)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_enum_payload_project_n3 = hxc_l_g.hxc_payload.hxc_JsonObject.hxc_fields;
  hxc_l_gc_roots[7] = (const void *)hxc_l_tmp_enum_payload_project_n3;
  struct hxc_array_ref *hxc_l_fields = hxc_l_tmp_enum_payload_project_n3;
  hxc_l_gc_roots[8] = (const void *)hxc_l_fields;
  struct hxc_array_ref *hxc_l_tmp_call_result_n5 = hxc_caxecraft_localization_RuntimeUiCatalog_readTextFields(hxc_l_reader, hxc_l_node, hxc_l_path, hxc_l_locales, hxc_l_fields);
  hxc_enum_3a7ff4b0_destroy(&hxc_l_g);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_call_result_n5;
}

struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_caxecraft_localization_RuntimeUiCatalog_rejected(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader)
{
  const void *hxc_l_gc_roots[2] = { NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 2, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  struct hxc_caxecraft_content_RuntimeSchemaDiagnostic hxc_l_tmp_enum_payload_0_owner_n3 = { 0 };
  if (hxc_l_reader == NULL)
  {
    abort();
  }
  struct hxc_optional_caxecraft_content_RuntimeSchemaDiagnostic hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_reader).hxc_failure;
  struct hxc_optional_caxecraft_content_RuntimeSchemaDiagnostic hxc_l_diagnostic = hxc_l_tmp_class_field_load_result_n0;
  if (hxc_optional_a967a598_retain(&hxc_l_diagnostic) != HXC_STATUS_OK)
  {
    abort();
  }
  if (!!hxc_l_diagnostic.hxc_has_value)
  {
    struct hxc_optional_caxecraft_content_RuntimeSchemaDiagnostic hxc_l_tmp_load_result_n6 = hxc_l_diagnostic;
    if (!hxc_l_tmp_load_result_n6.hxc_has_value)
    {
      abort();
    }
    hxc_l_tmp_enum_payload_0_owner_n3 = hxc_l_tmp_load_result_n6.hxc_value;
    if (hxc_record_c8d3a25f_retain(&hxc_l_tmp_enum_payload_0_owner_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_caxecraft_content_RuntimeSchemaDiagnostic hxc_l_tmp_enum_payload_0_owned_load_result_n8 = hxc_l_tmp_enum_payload_0_owner_n3;
    hxc_l_gc_roots[1] = (struct hxc_caxecraft_localization_RuntimeUiCatalogResult){ .hxc_tag = hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogRejected, .hxc_payload.hxc_RuntimeUiCatalogRejected.hxc_diagnostic = hxc_l_tmp_enum_payload_0_owned_load_result_n8 }.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)(struct hxc_caxecraft_localization_RuntimeUiCatalogResult){ .hxc_tag = hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogRejected, .hxc_payload.hxc_RuntimeUiCatalogRejected.hxc_diagnostic = hxc_l_tmp_enum_payload_0_owned_load_result_n8 }.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
    hxc_optional_a967a598_destroy(&hxc_l_diagnostic);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return (struct hxc_caxecraft_localization_RuntimeUiCatalogResult){ .hxc_tag = hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogRejected, .hxc_payload.hxc_RuntimeUiCatalogRejected.hxc_diagnostic = hxc_l_tmp_enum_payload_0_owned_load_result_n8 };
  }
  hxc_string hxc_l_tmp_enum_payload_0_owner_n2 = (hxc_string){ (const uint8_t *)"UI decoder", 10, true, NULL };
  if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_enum_payload_0_owned_load_result_n2 = hxc_l_tmp_enum_payload_0_owner_n2;
  hxc_l_gc_roots[0] = (struct hxc_caxecraft_localization_RuntimeUiCatalogResult){ .hxc_tag = hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogRejected, .hxc_payload.hxc_RuntimeUiCatalogRejected.hxc_diagnostic = (struct hxc_caxecraft_content_RuntimeSchemaDiagnostic){ .hxc_column = 1, .hxc_kind = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidInvariant, .hxc_payload.hxc_SchemaInvalidInvariant.hxc_path = hxc_l_tmp_enum_payload_0_owned_load_result_n2 }, .hxc_line = 1 } }.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)(struct hxc_caxecraft_localization_RuntimeUiCatalogResult){ .hxc_tag = hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogRejected, .hxc_payload.hxc_RuntimeUiCatalogRejected.hxc_diagnostic = (struct hxc_caxecraft_content_RuntimeSchemaDiagnostic){ .hxc_column = 1, .hxc_kind = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidInvariant, .hxc_payload.hxc_SchemaInvalidInvariant.hxc_path = hxc_l_tmp_enum_payload_0_owned_load_result_n2 }, .hxc_line = 1 } }.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
  hxc_optional_a967a598_destroy(&hxc_l_diagnostic);
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return (struct hxc_caxecraft_localization_RuntimeUiCatalogResult){ .hxc_tag = hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogRejected, .hxc_payload.hxc_RuntimeUiCatalogRejected.hxc_diagnostic = (struct hxc_caxecraft_content_RuntimeSchemaDiagnostic){ .hxc_column = 1, .hxc_kind = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidInvariant, .hxc_payload.hxc_SchemaInvalidInvariant.hxc_path = hxc_l_tmp_enum_payload_0_owned_load_result_n2 }, .hxc_line = 1 } };
}

bool hxc_caxecraft_localization_RuntimeUiCatalog_sameStrings(struct hxc_array_ref *hxc_l_left, struct hxc_array_ref *hxc_l_right)
{
  hxc_string hxc_l_tmp_array_element_owner_n5 = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n6 = { 0 };
  int32_t hxc_l_tmp_array_length_result_n0;
  if (hxc_array_ref_length(hxc_l_left, &hxc_l_tmp_array_length_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_tmp_array_length_result_n1;
  if (hxc_array_ref_length(hxc_l_right, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_tmp_array_length_result_n0 != hxc_l_tmp_array_length_result_n1)
  {
    return false;
  }
  int32_t hxc_l_g = 0;
  int32_t hxc_l_tmp_array_length_result_n2;
  if (hxc_array_ref_length(hxc_l_left, &hxc_l_tmp_array_length_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n2;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n3 = hxc_l_g;
    if (!(hxc_l_tmp_load_result_n3 < hxc_l_g1))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n5 = hxc_l_g;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n5, 1);
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n5;
    hxc_string hxc_l_tmp_array_get_result_n7;
    if (hxc_array_ref_get_copy(hxc_l_left, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n7) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n5 = hxc_l_tmp_array_get_result_n7;
    hxc_string hxc_l_tmp_array_element_borrow_result_n8 = hxc_l_tmp_array_element_owner_n5;
    hxc_string hxc_l_tmp_array_get_result_n10;
    if (hxc_array_ref_get_copy(hxc_l_right, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n10) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n6 = hxc_l_tmp_array_get_result_n10;
    hxc_string hxc_l_tmp_array_element_borrow_result_n11 = hxc_l_tmp_array_element_owner_n6;
    if (hxc_string_release(&hxc_l_tmp_array_element_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_array_element_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_array_element_borrow_result_n8.data == NULL || hxc_l_tmp_array_element_borrow_result_n11.data == NULL ? hxc_l_tmp_array_element_borrow_result_n8.data == hxc_l_tmp_array_element_borrow_result_n11.data : hxc_l_tmp_array_element_borrow_result_n8.byte_length == hxc_l_tmp_array_element_borrow_result_n11.byte_length && (hxc_l_tmp_array_element_borrow_result_n8.byte_length == 0 || memcmp(hxc_l_tmp_array_element_borrow_result_n8.data, hxc_l_tmp_array_element_borrow_result_n11.data, hxc_l_tmp_array_element_borrow_result_n8.byte_length) == 0)))
    {
      return false;
    }
  }
  return true;
}

int32_t hxc_caxecraft_localization_RuntimeUiCatalog_nextLocale(struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_self, int32_t hxc_l_locale)
{
  const void *hxc_l_gc_roots[1] = { (const void *)hxc_l_self };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  switch (hxc_l_locale) {
    case 0:
      {
        break;
      }
    case 1:
      {
        if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
        {
          abort();
        }
        return 0;
      }
    default:
      {
        if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
        {
          abort();
        }
        return 0;
      }
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_locales;
  int32_t hxc_l_tmp_array_length_result_n1;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n0, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  if (!(hxc_l_tmp_array_length_result_n1 > 1))
  {
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return 0;
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return 1;
}

hxc_string hxc_caxecraft_localization_RuntimeUiCatalog_text(struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_self, int32_t hxc_l_locale, int32_t hxc_l_message)
{
  const void *hxc_l_gc_roots[4] = { (const void *)hxc_l_self, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 4, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_array_element_owner_n9 = { 0 };
  int32_t hxc_l_tmp_call_result_n0 = hxc_caxecraft_localization_RuntimeUiCatalog_localeStorageCode(hxc_l_locale);
  int32_t hxc_l_localeCode = hxc_l_tmp_call_result_n0;
  int32_t hxc_l_tmp_call_result_n1 = hxc_caxecraft_localization_RuntimeUiCatalog_messageStorageCode(hxc_l_message);
  int32_t hxc_l_messageCode = hxc_l_tmp_call_result_n1;
  int32_t hxc_l_tmp_load_result_n2 = hxc_l_localeCode;
  bool hxc_l_tmp_short_circuit_result_n5 = hxc_l_tmp_load_result_n2 < 0;
  if (!(hxc_l_tmp_load_result_n2 < 0))
  {
    int32_t hxc_l_tmp_load_result_n3 = hxc_l_localeCode;
    if (hxc_l_self == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n4 = (*hxc_l_self).hxc_locales;
    int32_t hxc_l_tmp_array_length_result_n5;
    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n4, &hxc_l_tmp_array_length_result_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n5 = hxc_l_tmp_load_result_n3 >= hxc_l_tmp_array_length_result_n5;
  }
  bool hxc_l_tmp_short_circuit_load_result_n6 = hxc_l_tmp_short_circuit_result_n5;
  bool hxc_l_tmp_short_circuit_result_n6 = hxc_l_tmp_short_circuit_load_result_n6;
  if (!hxc_l_tmp_short_circuit_load_result_n6)
  {
    hxc_l_tmp_short_circuit_result_n6 = hxc_l_messageCode < 0;
  }
  bool hxc_l_tmp_short_circuit_load_result_n8 = hxc_l_tmp_short_circuit_result_n6;
  bool hxc_l_tmp_short_circuit_result_n7 = hxc_l_tmp_short_circuit_load_result_n8;
  if (!hxc_l_tmp_short_circuit_load_result_n8)
  {
    int32_t hxc_l_tmp_load_result_n9 = hxc_l_messageCode;
    if (hxc_l_self == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n10 = (*hxc_l_self).hxc_messages;
    hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n10;
    int32_t hxc_l_tmp_array_length_result_n11;
    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n10, &hxc_l_tmp_array_length_result_n11) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n7 = hxc_l_tmp_load_result_n9 >= hxc_l_tmp_array_length_result_n11;
  }
  if (!hxc_l_tmp_short_circuit_result_n7)
  {
    if (hxc_l_self == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n14 = (*hxc_l_self).hxc_messages;
    hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_class_field_load_result_n14;
    struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *hxc_l_tmp_array_get_result_n16;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n14, (size_t)hxc_l_messageCode, &hxc_l_tmp_array_get_result_n16) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_array_get_result_n16;
    if (hxc_l_tmp_array_get_result_n16 == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n17 = (*hxc_l_tmp_array_get_result_n16).hxc_texts;
    hxc_string hxc_l_tmp_array_get_result_n19;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n17, (size_t)hxc_l_localeCode, &hxc_l_tmp_array_get_result_n19) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n9 = hxc_l_tmp_array_get_result_n19;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_array_element_owner_n9;
  }
  hxc_string hxc_l_tmp_returned_string_owner_n8 = (hxc_string){ (const uint8_t *)"", 0, true, NULL };
  if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n8) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_returned_string_owner_n8;
}
