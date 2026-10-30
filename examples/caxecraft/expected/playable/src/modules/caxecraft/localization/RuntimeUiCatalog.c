#include "hxc/program.h"

void hxc_compiler_constructor_caxecraft_localization_RuntimeUiCatalog(struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_self, hxc_string hxc_l_catalogId, struct hxc_array_ref *hxc_l_locales, struct hxc_array_ref *hxc_l_messages, struct hxc_array_ref *hxc_l_templates)
{
  const void *volatile hxc_l_gc_roots[3] = { NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  hxc_l_gc_roots[1] = (const void *)hxc_l_messages;
  hxc_l_gc_roots[2] = (const void *)hxc_l_templates;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 3, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_TField_catalogIdValue_managed_String_initializer_owner_n5 = hxc_l_catalogId;
  if (hxc_string_retain(hxc_l_tmp_TField_catalogIdValue_managed_String_initializer_owner_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_catalogIdValue = hxc_l_tmp_TField_catalogIdValue_managed_String_initializer_owner_n5;
  (*hxc_l_self).hxc_locales = hxc_l_locales;
  if (hxc_array_ref_retain((*hxc_l_self).hxc_locales) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_messages = hxc_l_messages;
  (*hxc_l_self).hxc_templates = hxc_l_templates;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

void hxc_compiler_constructor_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition(struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *hxc_l_self, hxc_string hxc_l_id, struct hxc_array_ref *hxc_l_texts)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 1, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_TField_id_managed_String_initializer_owner_n3 = hxc_l_id;
  if (hxc_string_retain(hxc_l_tmp_TField_id_managed_String_initializer_owner_n3) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_id = hxc_l_tmp_TField_id_managed_String_initializer_owner_n3;
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

void hxc_compiler_constructor_caxecraft_localization_RuntimeUiCatalog_RuntimeUiTemplateDefinition(struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiTemplateDefinition *hxc_l_self, hxc_string hxc_l_id, struct hxc_array_ref *hxc_l_texts, int32_t hxc_l_argumentCount)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
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
  (*hxc_l_self).hxc_texts = hxc_l_texts;
  if (hxc_array_ref_retain((*hxc_l_self).hxc_texts) != HXC_STATUS_OK)
  {
    abort();
  }
  (*hxc_l_self).hxc_argumentCount = hxc_l_argumentCount;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_caxecraft_localization_RuntimeUiCatalog_decode(struct hxc_bytes_ref *hxc_l_input)
{
  const void *volatile hxc_l_gc_roots[94] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 94, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_enum_payload_0_owner_n12 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n13 = { 0 };
  hxc_string hxc_l_catalogId = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n18 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n19 = { 0 };
  struct hxc_array_ref *hxc_l_locales = { 0 };
  hxc_string hxc_l_value = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n28 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n29 = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n30 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n32 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n33 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n34 = { 0 };
  hxc_string hxc_l_existing_hc276e5de980d = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n37 = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n39 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n40 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n41 = { 0 };
  hxc_string hxc_l_defaultLocale = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n46 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n48 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n49 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_instance_call_argument_2_owner_n5 = { 0 };
  hxc_string hxc_l_path = { 0 };
  hxc_string hxc_l_tmp_string_concat_right_owner_n58 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n59 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_instance_call_argument_2_owner_n61 = { 0 };
  hxc_string hxc_l_id = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n65 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n67 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n71 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_1_owner_n72 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n73 = { 0 };
  hxc_string hxc_l_tmp_enum_payload_0_owner_n75 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n76 = { 0 };
  struct hxc_array_ref *hxc_l_texts = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n78 = { 0 };
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
  if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n8->value, &(hxc_string){ (const uint8_t *)"templates", 9, true, NULL }) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_instance_call_argument_2_owner_n5 = hxc_l_tmp_array_create_result_n8;
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n10 = hxc_caxecraft_content_RuntimeSchemaReader_object(hxc_l_tmp_load_result_n6, hxc_l_tmp_load_result_n7, (hxc_string){ (const uint8_t *)"UI catalog", 10, true, NULL }, hxc_l_tmp_instance_call_argument_2_owner_n5);
  hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_instance_call_result_n10;
  struct hxc_array_ref *hxc_l_fields = hxc_l_tmp_instance_call_result_n10;
  hxc_l_gc_roots[6] = (const void *)hxc_l_fields;
  hxc_l_gc_roots[7] = (const void *)NULL;
  bool hxc_l_tmp_array_reference_equality_result_n6 = hxc_l_fields == NULL;
  if (hxc_l_tmp_array_reference_equality_result_n6)
  {
    struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n14 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
    hxc_l_gc_roots[8] = hxc_l_tmp_call_result_n14.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n14.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n14;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n15 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n15 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[9] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n17 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n15, hxc_l_fields, (hxc_string){ (const uint8_t *)"schemaVersion", 13, true, NULL });
  hxc_l_gc_roots[10] = (const void *)hxc_l_tmp_instance_call_result_n17;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_versionNode = hxc_l_tmp_instance_call_result_n17;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n18 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n18 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[11] = (const void *)hxc_l_versionNode;
  struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_instance_call_result_n20 = hxc_caxecraft_content_RuntimeSchemaReader_integer(hxc_l_tmp_load_result_n18, hxc_l_versionNode, (hxc_string){ (const uint8_t *)"schemaVersion", 13, true, NULL }, 0, 2147483647);
  struct hxc_optional_value_h76f8e80e9d2b hxc_l_version = hxc_l_tmp_instance_call_result_n20;
  if (!hxc_l_version.hxc_has_value)
  {
    struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n23 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
    hxc_l_gc_roots[12] = hxc_l_tmp_call_result_n23.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n23.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n23;
  }
  struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_optional_scalar_equality_optional_n9 = hxc_l_version;
  int32_t hxc_l_tmp_optional_scalar_equality_scalar_n10 = 1;
  bool hxc_l_tmp_optional_scalar_equality_result_n11 = true;
  if (hxc_l_tmp_optional_scalar_equality_optional_n9.hxc_has_value)
  {
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_optional_scalar_compare_optional_load_result_n26 = hxc_l_tmp_optional_scalar_equality_optional_n9;
    int32_t hxc_l_tmp_optional_scalar_compare_scalar_load_result_n27 = hxc_l_tmp_optional_scalar_equality_scalar_n10;
    if (!hxc_l_tmp_optional_scalar_compare_optional_load_result_n26.hxc_has_value)
    {
      abort();
    }
    hxc_l_tmp_optional_scalar_equality_result_n11 = hxc_l_tmp_optional_scalar_compare_optional_load_result_n26.hxc_value != hxc_l_tmp_optional_scalar_compare_scalar_load_result_n27;
  }
  if (hxc_l_tmp_optional_scalar_equality_result_n11)
  {
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n29 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n29 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n30 = hxc_l_versionNode;
    hxc_l_gc_roots[13] = (const void *)hxc_l_tmp_load_result_n30;
    hxc_l_tmp_enum_payload_0_owner_n12 = (hxc_string){ (const uint8_t *)"schemaVersion", 13, true, NULL };
    if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n12) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n13 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaUnsupportedVersion, .hxc_payload.hxc_SchemaUnsupportedVersion.hxc_path = hxc_l_tmp_enum_payload_0_owner_n12, .hxc_payload.hxc_SchemaUnsupportedVersion.hxc_expected = 1 };
    hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n29, hxc_l_tmp_load_result_n30, hxc_l_tmp_instance_call_argument_1_owner_n13);
    struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n35 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
    hxc_l_gc_roots[14] = hxc_l_tmp_call_result_n35.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n35.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
    hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n13);
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_optional_a967a598_destroy(&hxc_l_tmp_object_storage_n2.hxc_failure);
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_call_result_n35;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n36 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n36 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[15] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n38 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n36, hxc_l_fields, (hxc_string){ (const uint8_t *)"catalogId", 9, true, NULL });
  hxc_l_gc_roots[16] = (const void *)hxc_l_tmp_instance_call_result_n38;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_catalogNode = hxc_l_tmp_instance_call_result_n38;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n39 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n39 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[17] = (const void *)hxc_l_catalogNode;
  hxc_string hxc_l_tmp_instance_call_result_n41 = hxc_caxecraft_content_RuntimeSchemaReader_string(hxc_l_tmp_load_result_n39, hxc_l_catalogNode, (hxc_string){ (const uint8_t *)"catalogId", 9, true, NULL }, 128);
  hxc_l_catalogId = hxc_l_tmp_instance_call_result_n41;
  bool hxc_l_tmp_string_null_equality_result_n16 = hxc_l_catalogId.data == NULL;
  if (hxc_l_tmp_string_null_equality_result_n16)
  {
    struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n45 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
    hxc_l_gc_roots[18] = hxc_l_tmp_call_result_n45.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n45.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
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
    return hxc_l_tmp_call_result_n45;
  }
  bool hxc_l_tmp_string_equality_result_n17 = !(hxc_l_catalogId.data != NULL && (hxc_l_catalogId.byte_length == (hxc_string){ (const uint8_t *)"caxecraft.ui", 12, true, NULL }.byte_length && (hxc_l_catalogId.byte_length == 0 || memcmp(hxc_l_catalogId.data, (hxc_string){ (const uint8_t *)"caxecraft.ui", 12, true, NULL }.data, hxc_l_catalogId.byte_length) == 0)));
  if (hxc_l_tmp_string_equality_result_n17)
  {
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n49 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n49 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n50 = hxc_l_catalogNode;
    hxc_l_gc_roots[19] = (const void *)hxc_l_tmp_load_result_n50;
    hxc_l_tmp_enum_payload_0_owner_n18 = (hxc_string){ (const uint8_t *)"catalogId", 9, true, NULL };
    if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n18) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n19 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaIncompatibleTypedCatalog, .hxc_payload.hxc_SchemaIncompatibleTypedCatalog.hxc_path = hxc_l_tmp_enum_payload_0_owner_n18 };
    hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n49, hxc_l_tmp_load_result_n50, hxc_l_tmp_instance_call_argument_1_owner_n19);
    struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n55 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
    hxc_l_gc_roots[20] = hxc_l_tmp_call_result_n55.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n55.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
    hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n19);
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
    return hxc_l_tmp_call_result_n55;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n56 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n56 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[21] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n58 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n56, hxc_l_fields, (hxc_string){ (const uint8_t *)"locales", 7, true, NULL });
  hxc_l_gc_roots[22] = (const void *)hxc_l_tmp_instance_call_result_n58;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_localeNode = hxc_l_tmp_instance_call_result_n58;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n59 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n59 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[23] = (const void *)hxc_l_localeNode;
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n61 = hxc_caxecraft_content_RuntimeSchemaReader_array(hxc_l_tmp_load_result_n59, hxc_l_localeNode, (hxc_string){ (const uint8_t *)"locales", 7, true, NULL }, 1, 8);
  hxc_l_gc_roots[24] = (const void *)hxc_l_tmp_instance_call_result_n61;
  struct hxc_array_ref *hxc_l_localeValues = hxc_l_tmp_instance_call_result_n61;
  hxc_l_gc_roots[25] = (const void *)hxc_l_localeValues;
  hxc_l_gc_roots[26] = (const void *)NULL;
  bool hxc_l_tmp_array_reference_equality_result_n22 = hxc_l_localeValues == NULL;
  if (hxc_l_tmp_array_reference_equality_result_n22)
  {
    struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n65 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
    hxc_l_gc_roots[27] = hxc_l_tmp_call_result_n65.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n65.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
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
    return hxc_l_tmp_call_result_n65;
  }
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n66 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n66) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_locales = hxc_l_tmp_array_create_result_n66;
  int32_t hxc_l_g_h09f02109415f = 0;
  hxc_l_gc_roots[28] = (const void *)hxc_l_localeValues;
  int32_t hxc_l_tmp_array_length_result_n68;
  if (hxc_array_ref_length(hxc_l_localeValues, &hxc_l_tmp_array_length_result_n68) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1_h54e9d7848582 = hxc_l_tmp_array_length_result_n68;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n69 = hxc_l_g_h09f02109415f;
    if (!(hxc_l_tmp_load_result_n69 < hxc_l_g1_h54e9d7848582))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n71 = hxc_l_g_h09f02109415f;
    hxc_l_g_h09f02109415f = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n71, 1);
    int32_t hxc_l_index_h440ea03a79be = hxc_l_tmp_increment_load_result_n71;
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n72 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n72 == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n73 = hxc_l_localeValues;
    hxc_l_gc_roots[29] = (const void *)hxc_l_tmp_load_result_n73;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_array_get_result_n75;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n73, (size_t)hxc_l_index_h440ea03a79be, &hxc_l_tmp_array_get_result_n75) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[30] = (const void *)hxc_l_tmp_array_get_result_n75;
    hxc_string hxc_l_tmp_std_string_int_result_n77 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_from_int32(hxc_l_index_h440ea03a79be, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n77) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_right_owner_n28 = hxc_l_tmp_std_string_int_result_n77;
    hxc_string hxc_l_tmp_string_concat_result_n79 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"locales[", 8, true, NULL }, hxc_l_tmp_string_concat_right_owner_n28, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n79) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_left_owner_n29 = hxc_l_tmp_string_concat_result_n79;
    hxc_string hxc_l_tmp_string_concat_result_n81 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n29, (hxc_string){ (const uint8_t *)"]", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n81) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n30 = hxc_l_tmp_string_concat_result_n81;
    hxc_string hxc_l_tmp_instance_call_result_n83 = hxc_caxecraft_content_RuntimeSchemaReader_string(hxc_l_tmp_load_result_n72, hxc_l_tmp_array_get_result_n75, hxc_l_tmp_instance_call_argument_1_owner_n30, 128);
    hxc_l_value = hxc_l_tmp_instance_call_result_n83;
    bool hxc_l_tmp_string_null_equality_result_n31 = hxc_l_value.data == NULL;
    if (hxc_l_tmp_string_null_equality_result_n31)
    {
      struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n87 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
      hxc_l_gc_roots[34] = hxc_l_tmp_call_result_n87.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n87.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
      if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n30) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n29) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n28) != HXC_STATUS_OK)
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
      return hxc_l_tmp_call_result_n87;
    }
    bool hxc_l_tmp_call_result_n89 = hxc_caxecraft_content_RuntimeSchemaReader_validLocale(hxc_l_value);
    if (!hxc_l_tmp_call_result_n89)
    {
      struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n90 = hxc_l_reader;
      if (hxc_l_tmp_load_result_n90 == NULL)
      {
        abort();
      }
      struct hxc_array_ref *hxc_l_tmp_load_result_n91 = hxc_l_localeValues;
      hxc_l_gc_roots[35] = (const void *)hxc_l_tmp_load_result_n91;
      struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_array_get_result_n93;
      if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n91, (size_t)hxc_l_index_h440ea03a79be, &hxc_l_tmp_array_get_result_n93) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_gc_roots[36] = (const void *)hxc_l_tmp_array_get_result_n93;
      hxc_string hxc_l_tmp_std_string_int_result_n95 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_from_int32(hxc_l_index_h440ea03a79be, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n95) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_string_concat_right_owner_n32 = hxc_l_tmp_std_string_int_result_n95;
      hxc_string hxc_l_tmp_string_concat_result_n97 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"locales[", 8, true, NULL }, hxc_l_tmp_string_concat_right_owner_n32, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n97) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_string_concat_left_owner_n33 = hxc_l_tmp_string_concat_result_n97;
      hxc_string hxc_l_tmp_string_concat_result_n99 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n33, (hxc_string){ (const uint8_t *)"]", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n99) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_instance_call_argument_1_owner_n34 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidLocale, .hxc_payload.hxc_SchemaInvalidLocale.hxc_path = hxc_l_tmp_string_concat_result_n99 };
      hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n90, hxc_l_tmp_array_get_result_n93, hxc_l_tmp_instance_call_argument_1_owner_n34);
      struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n103 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
      hxc_l_gc_roots[37] = hxc_l_tmp_call_result_n103.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n103.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
      hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n34);
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n33) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n32) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n30) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n29) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n28) != HXC_STATUS_OK)
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
      return hxc_l_tmp_call_result_n103;
    }
    int32_t hxc_l_g_ha8d173fb84c2 = 0;
    while (1)
    {
      int32_t hxc_l_tmp_load_result_n104 = hxc_l_g_ha8d173fb84c2;
      int32_t hxc_l_tmp_array_length_result_n106;
      if (hxc_array_ref_length(hxc_l_locales, &hxc_l_tmp_array_length_result_n106) != HXC_STATUS_OK)
      {
        abort();
      }
      if (!(hxc_l_tmp_load_result_n104 < hxc_l_tmp_array_length_result_n106))
      {
        break;
      }
      struct hxc_array_ref *hxc_l_tmp_load_result_n107 = hxc_l_locales;
      hxc_string hxc_l_tmp_array_get_result_n109;
      if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n107, (size_t)hxc_l_g_ha8d173fb84c2, &hxc_l_tmp_array_get_result_n109) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_array_element_owner_n37 = hxc_l_tmp_array_get_result_n109;
      hxc_l_existing_hc276e5de980d = hxc_l_tmp_array_element_owner_n37;
      if (hxc_string_retain(hxc_l_existing_hc276e5de980d) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_g_ha8d173fb84c2 = hxc_i32_add_wrapping(hxc_l_g_ha8d173fb84c2, 1);
      hxc_string hxc_l_tmp_load_result_n112 = hxc_l_existing_hc276e5de980d;
      bool hxc_l_tmp_string_equality_result_n38 = hxc_l_tmp_load_result_n112.data == NULL || hxc_l_value.data == NULL ? hxc_l_tmp_load_result_n112.data == hxc_l_value.data : hxc_l_tmp_load_result_n112.byte_length == hxc_l_value.byte_length && (hxc_l_tmp_load_result_n112.byte_length == 0 || memcmp(hxc_l_tmp_load_result_n112.data, hxc_l_value.data, hxc_l_tmp_load_result_n112.byte_length) == 0);
      if (hxc_l_tmp_string_equality_result_n38)
      {
        struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n116 = hxc_l_reader;
        if (hxc_l_tmp_load_result_n116 == NULL)
        {
          abort();
        }
        struct hxc_array_ref *hxc_l_tmp_load_result_n117 = hxc_l_localeValues;
        hxc_l_gc_roots[38] = (const void *)hxc_l_tmp_load_result_n117;
        struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_array_get_result_n119;
        if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n117, (size_t)hxc_l_index_h440ea03a79be, &hxc_l_tmp_array_get_result_n119) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_gc_roots[39] = (const void *)hxc_l_tmp_array_get_result_n119;
        hxc_string hxc_l_tmp_std_string_int_result_n121 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_from_int32(hxc_l_index_h440ea03a79be, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n121) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_string_concat_right_owner_n39 = hxc_l_tmp_std_string_int_result_n121;
        hxc_string hxc_l_tmp_string_concat_result_n123 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"locales[", 8, true, NULL }, hxc_l_tmp_string_concat_right_owner_n39, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n123) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_string_concat_left_owner_n40 = hxc_l_tmp_string_concat_result_n123;
        hxc_string hxc_l_tmp_string_concat_result_n125 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n40, (hxc_string){ (const uint8_t *)"]", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n125) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_instance_call_argument_1_owner_n41 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidLocale, .hxc_payload.hxc_SchemaInvalidLocale.hxc_path = hxc_l_tmp_string_concat_result_n125 };
        hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n116, hxc_l_tmp_array_get_result_n119, hxc_l_tmp_instance_call_argument_1_owner_n41);
        struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n129 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
        hxc_l_gc_roots[40] = hxc_l_tmp_call_result_n129.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n129.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
        hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n41);
        if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n40) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n39) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_existing_hc276e5de980d) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_array_element_owner_n37) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n30) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n29) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n28) != HXC_STATUS_OK)
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
        return hxc_l_tmp_call_result_n129;
      }
      if (hxc_string_release(&hxc_l_existing_hc276e5de980d) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_array_element_owner_n37) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n130 = hxc_l_locales;
    int32_t hxc_l_tmp_array_push_result_n132;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n130, &hxc_l_value, &hxc_l_tmp_array_push_result_n132) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n132;
    if (hxc_string_release(&hxc_l_value) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n30) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n29) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n28) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n133 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n133 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[31] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n135 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n133, hxc_l_fields, (hxc_string){ (const uint8_t *)"defaultLocale", 13, true, NULL });
  hxc_l_gc_roots[32] = (const void *)hxc_l_tmp_instance_call_result_n135;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_defaultNode = hxc_l_tmp_instance_call_result_n135;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n136 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n136 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[33] = (const void *)hxc_l_defaultNode;
  hxc_string hxc_l_tmp_instance_call_result_n138 = hxc_caxecraft_content_RuntimeSchemaReader_string(hxc_l_tmp_load_result_n136, hxc_l_defaultNode, (hxc_string){ (const uint8_t *)"defaultLocale", 13, true, NULL }, 128);
  hxc_l_defaultLocale = hxc_l_tmp_instance_call_result_n138;
  bool hxc_l_tmp_string_null_equality_result_n44 = hxc_l_defaultLocale.data == NULL;
  if (hxc_l_tmp_string_null_equality_result_n44)
  {
    struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n142 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
    hxc_l_gc_roots[41] = hxc_l_tmp_call_result_n142.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n142.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
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
    return hxc_l_tmp_call_result_n142;
  }
  bool hxc_l_tmp_call_result_n144 = hxc_caxecraft_content_RuntimeSchemaReader_validLocale(hxc_l_defaultLocale);
  bool hxc_l_tmp_short_circuit_result_n45 = !hxc_l_tmp_call_result_n144;
  if (!!hxc_l_tmp_call_result_n144)
  {
    hxc_string hxc_l_tmp_load_result_n145 = hxc_l_defaultLocale;
    hxc_string hxc_l_tmp_array_get_result_n147;
    if (hxc_array_ref_get_copy(hxc_l_locales, (size_t)0, &hxc_l_tmp_array_get_result_n147) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n46 = hxc_l_tmp_array_get_result_n147;
    bool hxc_l_tmp_string_equality_result_n47 = !(hxc_l_tmp_load_result_n145.data == NULL || hxc_l_tmp_array_element_owner_n46.data == NULL ? hxc_l_tmp_load_result_n145.data == hxc_l_tmp_array_element_owner_n46.data : hxc_l_tmp_load_result_n145.byte_length == hxc_l_tmp_array_element_owner_n46.byte_length && (hxc_l_tmp_load_result_n145.byte_length == 0 || memcmp(hxc_l_tmp_load_result_n145.data, hxc_l_tmp_array_element_owner_n46.data, hxc_l_tmp_load_result_n145.byte_length) == 0));
    hxc_l_tmp_short_circuit_result_n45 = hxc_l_tmp_string_equality_result_n47;
    if (hxc_string_release(&hxc_l_tmp_array_element_owner_n46) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  if (hxc_l_tmp_short_circuit_result_n45)
  {
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n152 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n152 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n153 = hxc_l_defaultNode;
    hxc_l_gc_roots[42] = (const void *)hxc_l_tmp_load_result_n153;
    hxc_l_tmp_enum_payload_0_owner_n48 = (hxc_string){ (const uint8_t *)"defaultLocale", 13, true, NULL };
    if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n48) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n49 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidLocale, .hxc_payload.hxc_SchemaInvalidLocale.hxc_path = hxc_l_tmp_enum_payload_0_owner_n48 };
    hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n152, hxc_l_tmp_load_result_n153, hxc_l_tmp_instance_call_argument_1_owner_n49);
    struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n158 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
    hxc_l_gc_roots[43] = hxc_l_tmp_call_result_n158.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n158.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
    hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n49);
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
    return hxc_l_tmp_call_result_n158;
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n159 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n159 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[44] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n161 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n159, hxc_l_fields, (hxc_string){ (const uint8_t *)"messages", 8, true, NULL });
  hxc_l_gc_roots[45] = (const void *)hxc_l_tmp_instance_call_result_n161;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_messageNode = hxc_l_tmp_instance_call_result_n161;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n162 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n162 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[46] = (const void *)hxc_l_messageNode;
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n164 = hxc_caxecraft_content_RuntimeSchemaReader_array(hxc_l_tmp_load_result_n162, hxc_l_messageNode, (hxc_string){ (const uint8_t *)"messages", 8, true, NULL }, 1, 128);
  hxc_l_gc_roots[47] = (const void *)hxc_l_tmp_instance_call_result_n164;
  struct hxc_array_ref *hxc_l_messageValues = hxc_l_tmp_instance_call_result_n164;
  hxc_l_gc_roots[48] = (const void *)hxc_l_messageValues;
  hxc_l_gc_roots[49] = (const void *)NULL;
  bool hxc_l_tmp_array_reference_equality_result_n52 = hxc_l_messageValues == NULL;
  if (hxc_l_tmp_array_reference_equality_result_n52)
  {
    struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n168 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
    hxc_l_gc_roots[50] = hxc_l_tmp_call_result_n168.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n168.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
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
    return hxc_l_tmp_call_result_n168;
  }
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n169 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_array_4fb4ef37_descriptor, (void **)&hxc_l_tmp_array_create_result_n169) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_init_in_place(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *), _Alignof(struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *), NULL, NULL, NULL, NULL }, hxc_l_tmp_array_create_result_n169) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[51] = (const void *)hxc_l_tmp_array_create_result_n169;
  struct hxc_array_ref *hxc_l_messages = hxc_l_tmp_array_create_result_n169;
  int32_t hxc_l_g_h2c8faee4a74e = 0;
  hxc_l_gc_roots[52] = (const void *)hxc_l_messageValues;
  int32_t hxc_l_tmp_array_length_result_n171;
  if (hxc_array_ref_length(hxc_l_messageValues, &hxc_l_tmp_array_length_result_n171) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1_hb3add964116b = hxc_l_tmp_array_length_result_n171;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n172 = hxc_l_g_h2c8faee4a74e;
    if (!(hxc_l_tmp_load_result_n172 < hxc_l_g1_hb3add964116b))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n174 = hxc_l_g_h2c8faee4a74e;
    hxc_l_g_h2c8faee4a74e = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n174, 1);
    int32_t hxc_l_index_h562506f58b6c = hxc_l_tmp_increment_load_result_n174;
    hxc_string hxc_l_tmp_std_string_int_result_n176 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_from_int32(hxc_l_index_h562506f58b6c, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n176) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_right_owner_n58 = hxc_l_tmp_std_string_int_result_n176;
    hxc_string hxc_l_tmp_string_concat_result_n178 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"messages[", 9, true, NULL }, hxc_l_tmp_string_concat_right_owner_n58, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n178) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_left_owner_n59 = hxc_l_tmp_string_concat_result_n178;
    hxc_string hxc_l_tmp_string_concat_result_n180 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n59, (hxc_string){ (const uint8_t *)"]", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n180) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_path = hxc_l_tmp_string_concat_result_n180;
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n181 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n181 == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n182 = hxc_l_messageValues;
    hxc_l_gc_roots[53] = (const void *)hxc_l_tmp_load_result_n182;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_array_get_result_n184;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n182, (size_t)hxc_l_index_h562506f58b6c, &hxc_l_tmp_array_get_result_n184) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[54] = (const void *)hxc_l_tmp_array_get_result_n184;
    hxc_string hxc_l_tmp_load_result_n185 = hxc_l_path;
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n186 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n186) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n186->value, &(hxc_string){ (const uint8_t *)"id", 2, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n186->value, &(hxc_string){ (const uint8_t *)"text", 4, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_2_owner_n61 = hxc_l_tmp_array_create_result_n186;
    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n188 = hxc_caxecraft_content_RuntimeSchemaReader_object(hxc_l_tmp_load_result_n181, hxc_l_tmp_array_get_result_n184, hxc_l_tmp_load_result_n185, hxc_l_tmp_instance_call_argument_2_owner_n61);
    hxc_l_gc_roots[55] = (const void *)hxc_l_tmp_instance_call_result_n188;
    struct hxc_array_ref *hxc_l_messageFields = hxc_l_tmp_instance_call_result_n188;
    hxc_l_gc_roots[56] = (const void *)hxc_l_messageFields;
    hxc_l_gc_roots[57] = (const void *)NULL;
    bool hxc_l_tmp_array_reference_equality_result_n62 = hxc_l_messageFields == NULL;
    if (hxc_l_tmp_array_reference_equality_result_n62)
    {
      struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n192 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
      hxc_l_gc_roots[63] = hxc_l_tmp_call_result_n192.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n192.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n61) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n59) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n58) != HXC_STATUS_OK)
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
      return hxc_l_tmp_call_result_n192;
    }
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n193 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n193 == NULL)
    {
      abort();
    }
    hxc_l_gc_roots[64] = (const void *)hxc_l_messageFields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n195 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n193, hxc_l_messageFields, (hxc_string){ (const uint8_t *)"id", 2, true, NULL });
    hxc_l_gc_roots[65] = (const void *)hxc_l_tmp_instance_call_result_n195;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_idNode = hxc_l_tmp_instance_call_result_n195;
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n196 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n196 == NULL)
    {
      abort();
    }
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n197 = hxc_l_idNode;
    hxc_l_gc_roots[66] = (const void *)hxc_l_tmp_load_result_n197;
    hxc_string hxc_l_tmp_string_concat_result_n199 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".id", 3, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n199) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n65 = hxc_l_tmp_string_concat_result_n199;
    hxc_string hxc_l_tmp_instance_call_result_n201 = hxc_caxecraft_content_RuntimeSchemaReader_string(hxc_l_tmp_load_result_n196, hxc_l_tmp_load_result_n197, hxc_l_tmp_instance_call_argument_1_owner_n65, 128);
    hxc_l_id = hxc_l_tmp_instance_call_result_n201;
    bool hxc_l_tmp_string_null_equality_result_n66 = hxc_l_id.data == NULL;
    if (hxc_l_tmp_string_null_equality_result_n66)
    {
      struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n205 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
      hxc_l_gc_roots[67] = hxc_l_tmp_call_result_n205.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n205.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
      if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n65) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n61) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n59) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n58) != HXC_STATUS_OK)
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
      return hxc_l_tmp_call_result_n205;
    }
    bool hxc_l_tmp_call_result_n207 = hxc_caxecraft_content_RuntimeSchemaReader_validMessageId(hxc_l_id);
    if (!hxc_l_tmp_call_result_n207)
    {
      struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n208 = hxc_l_reader;
      if (hxc_l_tmp_load_result_n208 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n209 = hxc_l_idNode;
      hxc_l_gc_roots[68] = (const void *)hxc_l_tmp_load_result_n209;
      hxc_string hxc_l_tmp_string_concat_result_n211 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".id", 3, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n211) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_instance_call_argument_1_owner_n67 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidString, .hxc_payload.hxc_SchemaInvalidString.hxc_path = hxc_l_tmp_string_concat_result_n211 };
      hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n208, hxc_l_tmp_load_result_n209, hxc_l_tmp_instance_call_argument_1_owner_n67);
      struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n215 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
      hxc_l_gc_roots[69] = hxc_l_tmp_call_result_n215.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n215.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
      hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n67);
      if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n65) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n61) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n59) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n58) != HXC_STATUS_OK)
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
      return hxc_l_tmp_call_result_n215;
    }
    int32_t hxc_l_g_h565434eefa80 = 0;
    while (1)
    {
      int32_t hxc_l_tmp_load_result_n216 = hxc_l_g_h565434eefa80;
      hxc_l_gc_roots[70] = (const void *)hxc_l_messages;
      int32_t hxc_l_tmp_array_length_result_n218;
      if (hxc_array_ref_length(hxc_l_messages, &hxc_l_tmp_array_length_result_n218) != HXC_STATUS_OK)
      {
        abort();
      }
      if (!(hxc_l_tmp_load_result_n216 < hxc_l_tmp_array_length_result_n218))
      {
        break;
      }
      struct hxc_array_ref *hxc_l_tmp_load_result_n219 = hxc_l_messages;
      hxc_l_gc_roots[71] = (const void *)hxc_l_tmp_load_result_n219;
      struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *hxc_l_tmp_array_get_result_n221;
      if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n219, (size_t)hxc_l_g_h565434eefa80, &hxc_l_tmp_array_get_result_n221) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_gc_roots[72] = (const void *)hxc_l_tmp_array_get_result_n221;
      struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *hxc_l_existing_h616fa550322a = hxc_l_tmp_array_get_result_n221;
      hxc_l_g_h565434eefa80 = hxc_i32_add_wrapping(hxc_l_g_h565434eefa80, 1);
      struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *hxc_l_tmp_load_result_n223 = hxc_l_existing_h616fa550322a;
      hxc_l_gc_roots[73] = (const void *)hxc_l_tmp_load_result_n223;
      if (hxc_l_tmp_load_result_n223 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_class_field_load_result_n224 = (*hxc_l_tmp_load_result_n223).hxc_id;
      bool hxc_l_tmp_string_equality_result_n70 = hxc_l_tmp_class_field_load_result_n224.data == NULL || hxc_l_id.data == NULL ? hxc_l_tmp_class_field_load_result_n224.data == hxc_l_id.data : hxc_l_tmp_class_field_load_result_n224.byte_length == hxc_l_id.byte_length && (hxc_l_tmp_class_field_load_result_n224.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n224.data, hxc_l_id.data, hxc_l_tmp_class_field_load_result_n224.byte_length) == 0);
      if (hxc_l_tmp_string_equality_result_n70)
      {
        struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n228 = hxc_l_reader;
        if (hxc_l_tmp_load_result_n228 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n229 = hxc_l_idNode;
        hxc_l_gc_roots[75] = (const void *)hxc_l_tmp_load_result_n229;
        hxc_l_tmp_enum_payload_0_owner_n71 = (hxc_string){ (const uint8_t *)"messages", 8, true, NULL };
        if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n71) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_string hxc_l_tmp_enum_payload_0_owned_load_result_n230 = hxc_l_tmp_enum_payload_0_owner_n71;
        hxc_l_tmp_enum_payload_1_owner_n72 = hxc_l_id;
        if (hxc_string_retain(hxc_l_tmp_enum_payload_1_owner_n72) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_instance_call_argument_1_owner_n73 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaDuplicateId, .hxc_payload.hxc_SchemaDuplicateId.hxc_path = hxc_l_tmp_enum_payload_0_owned_load_result_n230, .hxc_payload.hxc_SchemaDuplicateId.hxc_id = hxc_l_tmp_enum_payload_1_owner_n72 };
        hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n228, hxc_l_tmp_load_result_n229, hxc_l_tmp_instance_call_argument_1_owner_n73);
        struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n236 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
        hxc_l_gc_roots[76] = hxc_l_tmp_call_result_n236.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n236.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
        hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n73);
        if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n65) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n61) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n59) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n58) != HXC_STATUS_OK)
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
        return hxc_l_tmp_call_result_n236;
      }
    }
    hxc_l_gc_roots[74] = (const void *)hxc_l_messages;
    int32_t hxc_l_tmp_array_length_result_n238;
    if (hxc_array_ref_length(hxc_l_messages, &hxc_l_tmp_array_length_result_n238) != HXC_STATUS_OK)
    {
      abort();
    }
    bool hxc_l_tmp_short_circuit_result_n74 = hxc_l_tmp_array_length_result_n238 > 0;
    if (hxc_l_tmp_array_length_result_n238 > 0)
    {
      struct hxc_array_ref *hxc_l_tmp_load_result_n239 = hxc_l_messages;
      hxc_l_gc_roots[77] = (const void *)hxc_l_tmp_load_result_n239;
      hxc_l_gc_roots[78] = (const void *)hxc_l_messages;
      int32_t hxc_l_tmp_array_length_result_n241;
      if (hxc_array_ref_length(hxc_l_messages, &hxc_l_tmp_array_length_result_n241) != HXC_STATUS_OK)
      {
        abort();
      }
      struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *hxc_l_tmp_array_get_result_n242;
      if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n239, (size_t)hxc_i32_subtract_wrapping(hxc_l_tmp_array_length_result_n241, 1), &hxc_l_tmp_array_get_result_n242) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_gc_roots[79] = (const void *)hxc_l_tmp_array_get_result_n242;
      if (hxc_l_tmp_array_get_result_n242 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_class_field_load_result_n243 = (*hxc_l_tmp_array_get_result_n242).hxc_id;
      int32_t hxc_l_tmp_call_result_n245 = hxc_caxecraft_content_RuntimeSchemaReader_compareUtf8(hxc_l_tmp_class_field_load_result_n243, hxc_l_id);
      hxc_l_tmp_short_circuit_result_n74 = hxc_l_tmp_call_result_n245 > 0;
    }
    if (hxc_l_tmp_short_circuit_result_n74)
    {
      struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n247 = hxc_l_reader;
      if (hxc_l_tmp_load_result_n247 == NULL)
      {
        abort();
      }
      struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n248 = hxc_l_idNode;
      hxc_l_gc_roots[80] = (const void *)hxc_l_tmp_load_result_n248;
      hxc_l_tmp_enum_payload_0_owner_n75 = (hxc_string){ (const uint8_t *)"messages", 8, true, NULL };
      if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n75) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_instance_call_argument_1_owner_n76 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaNonCanonicalOrder, .hxc_payload.hxc_SchemaNonCanonicalOrder.hxc_path = hxc_l_tmp_enum_payload_0_owner_n75 };
      hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_load_result_n247, hxc_l_tmp_load_result_n248, hxc_l_tmp_instance_call_argument_1_owner_n76);
      struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n253 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
      hxc_l_gc_roots[81] = hxc_l_tmp_call_result_n253.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n253.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
      hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n76);
      if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n65) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n61) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n59) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n58) != HXC_STATUS_OK)
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
      return hxc_l_tmp_call_result_n253;
    }
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n254 = hxc_l_reader;
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n255 = hxc_l_reader;
    if (hxc_l_tmp_load_result_n255 == NULL)
    {
      abort();
    }
    hxc_l_gc_roots[82] = (const void *)hxc_l_messageFields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n257 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n255, hxc_l_messageFields, (hxc_string){ (const uint8_t *)"text", 4, true, NULL });
    hxc_l_gc_roots[83] = (const void *)hxc_l_tmp_instance_call_result_n257;
    hxc_string hxc_l_tmp_string_concat_result_n259 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".text", 5, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n259) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n78 = hxc_l_tmp_string_concat_result_n259;
    hxc_string hxc_l_tmp_static_call_argument_2_borrow_result_n260 = hxc_l_tmp_static_call_argument_2_owner_n78;
    struct hxc_array_ref *hxc_l_tmp_call_result_n262 = hxc_caxecraft_localization_RuntimeUiCatalog_readTexts(hxc_l_tmp_load_result_n254, hxc_l_tmp_instance_call_result_n257, hxc_l_tmp_static_call_argument_2_borrow_result_n260, hxc_l_locales);
    hxc_l_texts = hxc_l_tmp_call_result_n262;
    bool hxc_l_tmp_array_reference_equality_result_n79 = hxc_l_texts == NULL;
    if (hxc_l_tmp_array_reference_equality_result_n79)
    {
      struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n266 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
      hxc_l_gc_roots[84] = hxc_l_tmp_call_result_n266.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n266.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
      if (hxc_array_ref_release(hxc_l_texts) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n78) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n65) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n61) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n59) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n58) != HXC_STATUS_OK)
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
      return hxc_l_tmp_call_result_n266;
    }
    struct hxc_array_ref *hxc_l_tmp_load_result_n267 = hxc_l_messages;
    hxc_l_gc_roots[85] = (const void *)hxc_l_tmp_load_result_n267;
    hxc_string hxc_l_tmp_load_result_n268 = hxc_l_id;
    struct hxc_array_ref *hxc_l_tmp_load_result_n269 = hxc_l_texts;
    struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *hxc_l_tmp_managed_class_result_n270 = NULL;
    if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition_descriptor, (void **)&hxc_l_tmp_managed_class_result_n270) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[86] = (const void *)hxc_l_tmp_managed_class_result_n270;
    hxc_compiler_constructor_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition(hxc_l_tmp_managed_class_result_n270, hxc_l_tmp_load_result_n268, hxc_l_tmp_load_result_n269);
    hxc_l_gc_roots[87] = (const void *)hxc_l_tmp_managed_class_result_n270;
    int32_t hxc_l_tmp_array_push_result_n271;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n267, &(struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *){ hxc_l_tmp_managed_class_result_n270 }, &hxc_l_tmp_array_push_result_n271) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n271;
    if (hxc_array_ref_release(hxc_l_texts) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n78) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n65) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n61) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n59) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n58) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n272 = hxc_l_reader;
  struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_load_result_n273 = hxc_l_reader;
  if (hxc_l_tmp_load_result_n273 == NULL)
  {
    abort();
  }
  hxc_l_gc_roots[58] = (const void *)hxc_l_fields;
  struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n275 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_tmp_load_result_n273, hxc_l_fields, (hxc_string){ (const uint8_t *)"templates", 9, true, NULL });
  hxc_l_gc_roots[59] = (const void *)hxc_l_tmp_instance_call_result_n275;
  struct hxc_array_ref *hxc_l_tmp_call_result_n277 = hxc_caxecraft_localization_RuntimeUiCatalog_readTemplates(hxc_l_tmp_load_result_n272, hxc_l_tmp_instance_call_result_n275, hxc_l_locales);
  hxc_l_gc_roots[60] = (const void *)hxc_l_tmp_call_result_n277;
  struct hxc_array_ref *hxc_l_templates = hxc_l_tmp_call_result_n277;
  hxc_l_gc_roots[61] = (const void *)hxc_l_templates;
  hxc_l_gc_roots[62] = (const void *)NULL;
  bool hxc_l_tmp_array_reference_equality_result_n81 = hxc_l_templates == NULL;
  if (!hxc_l_tmp_array_reference_equality_result_n81)
  {
    hxc_string hxc_l_tmp_load_result_n282 = hxc_l_catalogId;
    struct hxc_array_ref *hxc_l_tmp_load_result_n283 = hxc_l_locales;
    struct hxc_array_ref *hxc_l_tmp_load_result_n284 = hxc_l_messages;
    hxc_l_gc_roots[89] = (const void *)hxc_l_tmp_load_result_n284;
    struct hxc_array_ref *hxc_l_tmp_load_result_n285 = hxc_l_templates;
    hxc_l_gc_roots[90] = (const void *)hxc_l_tmp_load_result_n285;
    struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_tmp_managed_class_result_n286 = NULL;
    if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_localization_RuntimeUiCatalog_descriptor, (void **)&hxc_l_tmp_managed_class_result_n286) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[91] = (const void *)hxc_l_tmp_managed_class_result_n286;
    hxc_compiler_constructor_caxecraft_localization_RuntimeUiCatalog(hxc_l_tmp_managed_class_result_n286, hxc_l_tmp_load_result_n282, hxc_l_tmp_load_result_n283, hxc_l_tmp_load_result_n284, hxc_l_tmp_load_result_n285);
    hxc_l_gc_roots[92] = (const void *)hxc_l_tmp_managed_class_result_n286;
    hxc_l_gc_roots[93] = (struct hxc_caxecraft_localization_RuntimeUiCatalogResult){ .hxc_tag = hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady, .hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog = hxc_l_tmp_managed_class_result_n286 }.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)(struct hxc_caxecraft_localization_RuntimeUiCatalogResult){ .hxc_tag = hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady, .hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog = hxc_l_tmp_managed_class_result_n286 }.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
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
    return (struct hxc_caxecraft_localization_RuntimeUiCatalogResult){ .hxc_tag = hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady, .hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog = hxc_l_tmp_managed_class_result_n286 };
  }
  struct hxc_caxecraft_localization_RuntimeUiCatalogResult hxc_l_tmp_call_result_n281 = hxc_caxecraft_localization_RuntimeUiCatalog_rejected(hxc_l_reader);
  hxc_l_gc_roots[88] = hxc_l_tmp_call_result_n281.hxc_tag == hxc_caxecraft_localization_RuntimeUiCatalogResult_RuntimeUiCatalogReady ? (const void *)hxc_l_tmp_call_result_n281.hxc_payload.hxc_RuntimeUiCatalogReady.hxc_catalog : NULL;
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
  return hxc_l_tmp_call_result_n281;
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

int32_t hxc_caxecraft_localization_RuntimeUiCatalog_placeholderArgumentCount(struct hxc_array_ref *hxc_l_texts)
{
  hxc_string hxc_l_tmp_array_element_owner_n12 = { 0 };
  hxc_string hxc_l_text = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n3 = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n8 = { 0 };
  int32_t hxc_l_tmp_array_length_result_n0;
  if (hxc_array_ref_length(hxc_l_texts, &hxc_l_tmp_array_length_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_l_tmp_array_length_result_n0 == 0)
  {
    return -1;
  }
  int32_t hxc_l_g_hba4855c2c6cb = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n1 = hxc_l_g_hba4855c2c6cb;
    int32_t hxc_l_tmp_array_length_result_n2;
    if (hxc_array_ref_length(hxc_l_texts, &hxc_l_tmp_array_length_result_n2) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n1 < hxc_l_tmp_array_length_result_n2))
    {
      break;
    }
    hxc_string hxc_l_tmp_array_get_result_n4;
    if (hxc_array_ref_get_copy(hxc_l_texts, (size_t)hxc_l_g_hba4855c2c6cb, &hxc_l_tmp_array_get_result_n4) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n3 = hxc_l_tmp_array_get_result_n4;
    hxc_l_text = hxc_l_tmp_array_element_owner_n3;
    if (hxc_string_retain(hxc_l_text) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g_hba4855c2c6cb = hxc_i32_add_wrapping(hxc_l_g_hba4855c2c6cb, 1);
    bool hxc_l_tmp_call_result_n8 = hxc_caxecraft_localization_RuntimeUiCatalog_validPlaceholderTokens(hxc_l_text);
    if (!hxc_l_tmp_call_result_n8)
    {
      if (hxc_string_release(&hxc_l_text) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_array_element_owner_n3) != HXC_STATUS_OK)
      {
        abort();
      }
      return -1;
    }
    if (hxc_string_release(&hxc_l_text) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_array_element_owner_n3) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  int32_t hxc_l_argumentCount = 0;
  int32_t hxc_l_g_h17212d5180d3 = 0;
  while (1)
  {
    if (!(hxc_l_g_h17212d5180d3 < 10))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n10 = hxc_l_g_h17212d5180d3;
    hxc_l_g_h17212d5180d3 = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n10, 1);
    int32_t hxc_l_slot = hxc_l_tmp_increment_load_result_n10;
    hxc_string hxc_l_tmp_array_get_result_n11;
    if (hxc_array_ref_get_copy(hxc_l_texts, (size_t)0, &hxc_l_tmp_array_get_result_n11) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n8 = hxc_l_tmp_array_get_result_n11;
    hxc_string hxc_l_tmp_array_element_borrow_result_n12 = hxc_l_tmp_array_element_owner_n8;
    int32_t hxc_l_tmp_call_result_n14 = hxc_caxecraft_localization_RuntimeUiCatalog_placeholderCount(hxc_l_tmp_array_element_borrow_result_n12, hxc_l_slot);
    int32_t hxc_l_expected = hxc_l_tmp_call_result_n14;
    int32_t hxc_l_g_hb4c15a0d03b0 = 1;
    int32_t hxc_l_tmp_array_length_result_n15;
    if (hxc_array_ref_length(hxc_l_texts, &hxc_l_tmp_array_length_result_n15) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n15;
    while (1)
    {
      int32_t hxc_l_tmp_load_result_n16 = hxc_l_g_hb4c15a0d03b0;
      if (!(hxc_l_tmp_load_result_n16 < hxc_l_g1))
      {
        break;
      }
      int32_t hxc_l_tmp_increment_load_result_n18 = hxc_l_g_hb4c15a0d03b0;
      hxc_l_g_hb4c15a0d03b0 = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n18, 1);
      int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n18;
      hxc_string hxc_l_tmp_array_get_result_n20;
      if (hxc_array_ref_get_copy(hxc_l_texts, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n20) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_array_element_owner_n12 = hxc_l_tmp_array_get_result_n20;
      hxc_string hxc_l_tmp_array_element_borrow_result_n21 = hxc_l_tmp_array_element_owner_n12;
      int32_t hxc_l_tmp_call_result_n23 = hxc_caxecraft_localization_RuntimeUiCatalog_placeholderCount(hxc_l_tmp_array_element_borrow_result_n21, hxc_l_slot);
      int32_t hxc_l_tmp_load_result_n24 = hxc_l_expected;
      if (hxc_string_release(&hxc_l_tmp_array_element_owner_n12) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_l_tmp_call_result_n23 != hxc_l_tmp_load_result_n24)
      {
        if (hxc_string_release(&hxc_l_tmp_array_element_owner_n8) != HXC_STATUS_OK)
        {
          abort();
        }
        return -1;
      }
    }
    if (hxc_l_expected > 0)
    {
      int32_t hxc_l_tmp_load_result_n26 = hxc_l_slot;
      if (hxc_l_tmp_load_result_n26 != hxc_l_argumentCount)
      {
        if (hxc_string_release(&hxc_l_tmp_array_element_owner_n8) != HXC_STATUS_OK)
        {
          abort();
        }
        return -1;
      }
      else
      {
        hxc_l_argumentCount = hxc_i32_add_wrapping(hxc_l_argumentCount, 1);
      }
    }
    if (hxc_string_release(&hxc_l_tmp_array_element_owner_n8) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  return hxc_l_argumentCount;
}

int32_t hxc_caxecraft_localization_RuntimeUiCatalog_placeholderCount(hxc_string hxc_l_text, int32_t hxc_l_slot)
{
  hxc_string hxc_l_tmp_std_string_int_result_n0 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_from_int32(hxc_l_slot, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n0) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_string_concat_right_owner_n3 = hxc_l_tmp_std_string_int_result_n0;
  hxc_string hxc_l_tmp_string_concat_result_n2 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"{", 1, true, NULL }, hxc_l_tmp_string_concat_right_owner_n3, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n2) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_string_concat_left_owner_n4 = hxc_l_tmp_string_concat_result_n2;
  hxc_string hxc_l_tmp_string_concat_result_n4 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
  if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n4, (hxc_string){ (const uint8_t *)"}", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n4) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_token = hxc_l_tmp_string_concat_result_n4;
  int32_t hxc_l_count = 0;
  int32_t hxc_l_offset = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n5 = hxc_l_offset;
    if (hxc_l_text.data == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_string_length_result_n6;
    if (hxc_string_haxe_length(hxc_l_text, &hxc_l_tmp_string_length_result_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_string hxc_l_tmp_load_result_n7 = hxc_l_token;
    if (hxc_l_tmp_load_result_n7.data == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_string_length_result_n8;
    if (hxc_string_haxe_length(hxc_l_tmp_load_result_n7, &hxc_l_tmp_string_length_result_n8) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n5 <= hxc_i32_subtract_wrapping(hxc_l_tmp_string_length_result_n6, hxc_l_tmp_string_length_result_n8)))
    {
      break;
    }
    hxc_string hxc_l_tmp_load_result_n9 = hxc_l_token;
    if (hxc_l_tmp_load_result_n9.data == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_string_index_of_result_n11;
    if (hxc_string_index_of(hxc_l_text, hxc_l_tmp_load_result_n9, hxc_l_offset, &hxc_l_tmp_string_index_of_result_n11) != HXC_STATUS_OK)
    {
      abort();
    }
    int32_t hxc_l_found = hxc_l_tmp_string_index_of_result_n11;
    if (!(hxc_l_found < 0))
    {
      hxc_l_count = hxc_i32_add_wrapping(hxc_l_count, 1);
      int32_t hxc_l_tmp_load_result_n14 = hxc_l_found;
      hxc_string hxc_l_tmp_load_result_n15 = hxc_l_token;
      if (hxc_l_tmp_load_result_n15.data == NULL)
      {
        abort();
      }
      int32_t hxc_l_tmp_string_length_result_n16;
      if (hxc_string_haxe_length(hxc_l_tmp_load_result_n15, &hxc_l_tmp_string_length_result_n16) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_offset = hxc_i32_add_wrapping(hxc_l_tmp_load_result_n14, hxc_l_tmp_string_length_result_n16);
      continue;
    }
    int32_t hxc_l_tmp_load_result_n17 = hxc_l_count;
    if (hxc_string_release(&hxc_l_token) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n4) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_load_result_n17;
  }
  int32_t hxc_l_tmp_load_result_n17 = hxc_l_count;
  if (hxc_string_release(&hxc_l_token) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n4) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n3) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_load_result_n17;
}

struct hxc_array_ref *hxc_caxecraft_localization_RuntimeUiCatalog_readTemplates(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, struct hxc_caxecraft_content_ContentJsonNode *hxc_l_node, struct hxc_array_ref *hxc_l_locales)
{
  const void *volatile hxc_l_gc_roots[39] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_node;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 39, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_string_concat_right_owner_n10 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n11 = { 0 };
  struct hxc_array_ref *hxc_l_tmp_instance_call_argument_2_owner_n13 = { 0 };
  hxc_string hxc_l_id = { 0 };
  hxc_string hxc_l_tmp_instance_call_argument_1_owner_n17 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n19 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n28 = { 0 };
  struct hxc_array_ref *hxc_l_texts = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n30 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n33 = { 0 };
  hxc_string hxc_l_path = { 0 };
  if (hxc_l_reader == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_instance_call_result_n0 = hxc_caxecraft_content_RuntimeSchemaReader_array(hxc_l_reader, hxc_l_node, (hxc_string){ (const uint8_t *)"templates", 9, true, NULL }, 1, 256);
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_instance_call_result_n0;
  struct hxc_array_ref *hxc_l_values = hxc_l_tmp_instance_call_result_n0;
  hxc_l_gc_roots[2] = (const void *)hxc_l_values;
  hxc_l_gc_roots[3] = (const void *)NULL;
  bool hxc_l_tmp_array_reference_equality_result_n4 = hxc_l_values == NULL;
  if (hxc_l_tmp_array_reference_equality_result_n4)
  {
    hxc_l_gc_roots[4] = (const void *)NULL;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n3 = NULL;
  if (hxc_gc_allocate(&hxc_program_gc, &hxc_array_1ec24080_descriptor, (void **)&hxc_l_tmp_array_create_result_n3) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_array_ref_init_in_place(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiTemplateDefinition *), _Alignof(struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiTemplateDefinition *), NULL, NULL, NULL, NULL }, hxc_l_tmp_array_create_result_n3) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_array_create_result_n3;
  struct hxc_array_ref *hxc_l_result = hxc_l_tmp_array_create_result_n3;
  int32_t hxc_l_g = 0;
  hxc_l_gc_roots[6] = (const void *)hxc_l_values;
  int32_t hxc_l_tmp_array_length_result_n5;
  if (hxc_array_ref_length(hxc_l_values, &hxc_l_tmp_array_length_result_n5) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n5;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n6 = hxc_l_g;
    if (!(hxc_l_tmp_load_result_n6 < hxc_l_g1))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n8 = hxc_l_g;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n8, 1);
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n8;
    hxc_string hxc_l_tmp_std_string_int_result_n10 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_from_int32(hxc_l_index, hxc_default_allocator(), &hxc_l_tmp_std_string_int_result_n10) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_right_owner_n10 = hxc_l_tmp_std_string_int_result_n10;
    hxc_string hxc_l_tmp_string_concat_result_n12 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref((hxc_string){ (const uint8_t *)"templates[", 10, true, NULL }, hxc_l_tmp_string_concat_right_owner_n10, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n12) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_left_owner_n11 = hxc_l_tmp_string_concat_result_n12;
    hxc_string hxc_l_tmp_string_concat_result_n14 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_owner_n11, (hxc_string){ (const uint8_t *)"]", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n14) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_path = hxc_l_tmp_string_concat_result_n14;
    struct hxc_array_ref *hxc_l_tmp_load_result_n15 = hxc_l_values;
    hxc_l_gc_roots[7] = (const void *)hxc_l_tmp_load_result_n15;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_array_get_result_n17;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n15, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n17) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[8] = (const void *)hxc_l_tmp_array_get_result_n17;
    hxc_string hxc_l_tmp_load_result_n18 = hxc_l_path;
    struct hxc_array_ref *hxc_l_tmp_array_create_result_n19 = NULL;
    if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n19) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n19->value, &(hxc_string){ (const uint8_t *)"id", 2, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_push_copy(&hxc_l_tmp_array_create_result_n19->value, &(hxc_string){ (const uint8_t *)"text", 4, true, NULL }) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_2_owner_n13 = hxc_l_tmp_array_create_result_n19;
    struct hxc_array_ref *hxc_l_tmp_instance_call_result_n21 = hxc_caxecraft_content_RuntimeSchemaReader_object(hxc_l_reader, hxc_l_tmp_array_get_result_n17, hxc_l_tmp_load_result_n18, hxc_l_tmp_instance_call_argument_2_owner_n13);
    hxc_l_gc_roots[9] = (const void *)hxc_l_tmp_instance_call_result_n21;
    struct hxc_array_ref *hxc_l_fields = hxc_l_tmp_instance_call_result_n21;
    hxc_l_gc_roots[10] = (const void *)hxc_l_fields;
    hxc_l_gc_roots[11] = (const void *)NULL;
    bool hxc_l_tmp_array_reference_equality_result_n14 = hxc_l_fields == NULL;
    if (hxc_l_tmp_array_reference_equality_result_n14)
    {
      hxc_l_gc_roots[13] = (const void *)NULL;
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n13) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n11) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n10) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
    hxc_l_gc_roots[14] = (const void *)hxc_l_fields;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n25 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"id", 2, true, NULL });
    hxc_l_gc_roots[15] = (const void *)hxc_l_tmp_instance_call_result_n25;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_idNode = hxc_l_tmp_instance_call_result_n25;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n26 = hxc_l_idNode;
    hxc_l_gc_roots[16] = (const void *)hxc_l_tmp_load_result_n26;
    hxc_string hxc_l_tmp_string_concat_result_n28 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".id", 3, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n28) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_instance_call_argument_1_owner_n17 = hxc_l_tmp_string_concat_result_n28;
    hxc_string hxc_l_tmp_instance_call_result_n30 = hxc_caxecraft_content_RuntimeSchemaReader_string(hxc_l_reader, hxc_l_tmp_load_result_n26, hxc_l_tmp_instance_call_argument_1_owner_n17, 128);
    hxc_l_id = hxc_l_tmp_instance_call_result_n30;
    bool hxc_l_tmp_string_null_equality_result_n18 = hxc_l_id.data == NULL;
    if (hxc_l_tmp_string_null_equality_result_n18)
    {
      hxc_l_gc_roots[17] = (const void *)NULL;
      if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n17) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n13) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n11) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n10) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
    bool hxc_l_tmp_call_result_n34 = hxc_caxecraft_content_RuntimeSchemaReader_validMessageId(hxc_l_id);
    if (!hxc_l_tmp_call_result_n34)
    {
      struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n35 = hxc_l_idNode;
      hxc_l_gc_roots[18] = (const void *)hxc_l_tmp_load_result_n35;
      hxc_string hxc_l_tmp_string_concat_result_n37 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".id", 3, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n37) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_instance_call_argument_1_owner_n19 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidString, .hxc_payload.hxc_SchemaInvalidString.hxc_path = hxc_l_tmp_string_concat_result_n37 };
      hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_reader, hxc_l_tmp_load_result_n35, hxc_l_tmp_instance_call_argument_1_owner_n19);
      hxc_l_gc_roots[19] = (const void *)NULL;
      hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n19);
      if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n17) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n13) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n11) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n10) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return NULL;
    }
    hxc_l_gc_roots[20] = (const void *)hxc_l_result;
    int32_t hxc_l_tmp_array_length_result_n41;
    if (hxc_array_ref_length(hxc_l_result, &hxc_l_tmp_array_length_result_n41) != HXC_STATUS_OK)
    {
      abort();
    }
    bool hxc_l_tmp_short_circuit_result_n20 = hxc_l_tmp_array_length_result_n41 > 0;
    if (hxc_l_tmp_array_length_result_n41 > 0)
    {
      struct hxc_array_ref *hxc_l_tmp_load_result_n42 = hxc_l_result;
      hxc_l_gc_roots[21] = (const void *)hxc_l_tmp_load_result_n42;
      hxc_l_gc_roots[22] = (const void *)hxc_l_result;
      int32_t hxc_l_tmp_array_length_result_n44;
      if (hxc_array_ref_length(hxc_l_result, &hxc_l_tmp_array_length_result_n44) != HXC_STATUS_OK)
      {
        abort();
      }
      struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiTemplateDefinition *hxc_l_tmp_array_get_result_n45;
      if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n42, (size_t)hxc_i32_subtract_wrapping(hxc_l_tmp_array_length_result_n44, 1), &hxc_l_tmp_array_get_result_n45) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_gc_roots[23] = (const void *)hxc_l_tmp_array_get_result_n45;
      if (hxc_l_tmp_array_get_result_n45 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_class_field_load_result_n46 = (*hxc_l_tmp_array_get_result_n45).hxc_id;
      int32_t hxc_l_tmp_call_result_n48 = hxc_caxecraft_content_RuntimeSchemaReader_compareUtf8(hxc_l_tmp_class_field_load_result_n46, hxc_l_id);
      hxc_l_tmp_short_circuit_result_n20 = hxc_l_tmp_call_result_n48 >= 0;
    }
    if (!hxc_l_tmp_short_circuit_result_n20)
    {
      hxc_l_gc_roots[28] = (const void *)hxc_l_fields;
      struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_result_n70 = hxc_caxecraft_content_RuntimeSchemaReader_field(hxc_l_reader, hxc_l_fields, (hxc_string){ (const uint8_t *)"text", 4, true, NULL });
      hxc_l_gc_roots[29] = (const void *)hxc_l_tmp_instance_call_result_n70;
      hxc_string hxc_l_tmp_string_concat_result_n72 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".text", 5, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n72) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_static_call_argument_2_owner_n30 = hxc_l_tmp_string_concat_result_n72;
      struct hxc_array_ref *hxc_l_tmp_call_result_n74 = hxc_caxecraft_localization_RuntimeUiCatalog_readTexts(hxc_l_reader, hxc_l_tmp_instance_call_result_n70, hxc_l_tmp_static_call_argument_2_owner_n30, hxc_l_locales);
      hxc_l_texts = hxc_l_tmp_call_result_n74;
      bool hxc_l_tmp_array_reference_equality_result_n31 = hxc_l_texts == NULL;
      if (hxc_l_tmp_array_reference_equality_result_n31)
      {
        hxc_l_gc_roots[32] = (const void *)NULL;
        if (hxc_array_ref_release(hxc_l_texts) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n30) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n17) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n13) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n11) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n10) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
        {
          abort();
        }
        return NULL;
      }
      int32_t hxc_l_tmp_call_result_n78 = hxc_caxecraft_localization_RuntimeUiCatalog_placeholderArgumentCount(hxc_l_texts);
      int32_t hxc_l_argumentCount = hxc_l_tmp_call_result_n78;
      if (hxc_l_argumentCount < 0)
      {
        struct hxc_array_ref *hxc_l_tmp_load_result_n80 = hxc_l_values;
        hxc_l_gc_roots[33] = (const void *)hxc_l_tmp_load_result_n80;
        struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_array_get_result_n82;
        if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n80, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n82) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_gc_roots[34] = (const void *)hxc_l_tmp_array_get_result_n82;
        hxc_string hxc_l_tmp_string_concat_result_n84 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
        if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".text", 5, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n84) != HXC_STATUS_OK)
        {
          abort();
        }
        hxc_l_tmp_instance_call_argument_1_owner_n33 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaIncompatibleTypedCatalog, .hxc_payload.hxc_SchemaIncompatibleTypedCatalog.hxc_path = hxc_l_tmp_string_concat_result_n84 };
        hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_reader, hxc_l_tmp_array_get_result_n82, hxc_l_tmp_instance_call_argument_1_owner_n33);
        hxc_l_gc_roots[35] = (const void *)NULL;
        hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n33);
        if (hxc_array_ref_release(hxc_l_texts) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n30) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n17) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n13) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n11) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n10) != HXC_STATUS_OK)
        {
          abort();
        }
        if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
        {
          abort();
        }
        return NULL;
      }
      struct hxc_array_ref *hxc_l_tmp_load_result_n87 = hxc_l_result;
      hxc_l_gc_roots[36] = (const void *)hxc_l_tmp_load_result_n87;
      hxc_string hxc_l_tmp_load_result_n88 = hxc_l_id;
      struct hxc_array_ref *hxc_l_tmp_load_result_n89 = hxc_l_texts;
      int32_t hxc_l_tmp_load_result_n90 = hxc_l_argumentCount;
      struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiTemplateDefinition *hxc_l_tmp_managed_class_result_n91 = NULL;
      if (hxc_gc_allocate(&hxc_program_gc, &hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiTemplateDefinition_descriptor, (void **)&hxc_l_tmp_managed_class_result_n91) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_gc_roots[37] = (const void *)hxc_l_tmp_managed_class_result_n91;
      hxc_compiler_constructor_caxecraft_localization_RuntimeUiCatalog_RuntimeUiTemplateDefinition(hxc_l_tmp_managed_class_result_n91, hxc_l_tmp_load_result_n88, hxc_l_tmp_load_result_n89, hxc_l_tmp_load_result_n90);
      hxc_l_gc_roots[38] = (const void *)hxc_l_tmp_managed_class_result_n91;
      int32_t hxc_l_tmp_array_push_result_n92;
      if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n87, &(struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiTemplateDefinition *){ hxc_l_tmp_managed_class_result_n91 }, &hxc_l_tmp_array_push_result_n92) != HXC_STATUS_OK)
      {
        abort();
      }
      (void)hxc_l_tmp_array_push_result_n92;
      if (hxc_array_ref_release(hxc_l_texts) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n30) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n17) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n13) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n11) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n10) != HXC_STATUS_OK)
      {
        abort();
      }
      continue;
    }
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_instance_call_receiver_n21 = hxc_l_reader;
    hxc_l_gc_roots[24] = (const void *)hxc_l_idNode;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_argument_0_n22 = hxc_l_idNode;
    struct hxc_array_ref *hxc_l_tmp_load_result_n51 = hxc_l_result;
    hxc_l_gc_roots[25] = (const void *)hxc_l_tmp_load_result_n51;
    hxc_l_gc_roots[26] = (const void *)hxc_l_result;
    int32_t hxc_l_tmp_array_length_result_n53;
    if (hxc_array_ref_length(hxc_l_result, &hxc_l_tmp_array_length_result_n53) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiTemplateDefinition *hxc_l_tmp_array_get_result_n54;
    if (hxc_array_ref_get_copy(hxc_l_tmp_load_result_n51, (size_t)hxc_i32_subtract_wrapping(hxc_l_tmp_array_length_result_n53, 1), &hxc_l_tmp_array_get_result_n54) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[27] = (const void *)hxc_l_tmp_array_get_result_n54;
    if (hxc_l_tmp_array_get_result_n54 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n55 = (*hxc_l_tmp_array_get_result_n54).hxc_id;
    bool hxc_l_tmp_string_equality_result_n23 = hxc_l_tmp_class_field_load_result_n55.data == NULL || hxc_l_id.data == NULL ? hxc_l_tmp_class_field_load_result_n55.data == hxc_l_id.data : hxc_l_tmp_class_field_load_result_n55.byte_length == hxc_l_id.byte_length && (hxc_l_tmp_class_field_load_result_n55.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n55.data, hxc_l_id.data, hxc_l_tmp_class_field_load_result_n55.byte_length) == 0);
    bool hxc_l_tmp_string_equality_result_result_n58 = hxc_l_tmp_string_equality_result_n23;
    struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_conditional_managed_result_n24 = { 0 };
    if (hxc_l_tmp_string_equality_result_result_n58)
    {
      hxc_string hxc_l_tmp_enum_payload_0_owner_n25 = (hxc_string){ (const uint8_t *)"templates", 9, true, NULL };
      if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n25) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_string hxc_l_tmp_enum_payload_0_owned_load_result_n59 = hxc_l_tmp_enum_payload_0_owner_n25;
      hxc_string hxc_l_tmp_enum_payload_1_owner_n26 = hxc_l_id;
      if (hxc_string_retain(hxc_l_tmp_enum_payload_1_owner_n26) != HXC_STATUS_OK)
      {
        abort();
      }
      (void)(struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaDuplicateId, .hxc_payload.hxc_SchemaDuplicateId.hxc_path = hxc_l_tmp_enum_payload_0_owned_load_result_n59, .hxc_payload.hxc_SchemaDuplicateId.hxc_id = hxc_l_tmp_enum_payload_1_owner_n26 };
      hxc_l_tmp_conditional_managed_result_n24 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaDuplicateId, .hxc_payload.hxc_SchemaDuplicateId.hxc_path = hxc_l_tmp_enum_payload_0_owned_load_result_n59, .hxc_payload.hxc_SchemaDuplicateId.hxc_id = hxc_l_tmp_enum_payload_1_owner_n26 };
    }
    else
    {
      hxc_string hxc_l_tmp_enum_payload_0_owner_n27 = (hxc_string){ (const uint8_t *)"templates", 9, true, NULL };
      if (hxc_string_retain(hxc_l_tmp_enum_payload_0_owner_n27) != HXC_STATUS_OK)
      {
        abort();
      }
      (void)(struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaNonCanonicalOrder, .hxc_payload.hxc_SchemaNonCanonicalOrder.hxc_path = hxc_l_tmp_enum_payload_0_owner_n27 };
      hxc_l_tmp_conditional_managed_result_n24 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaNonCanonicalOrder, .hxc_payload.hxc_SchemaNonCanonicalOrder.hxc_path = hxc_l_tmp_enum_payload_0_owner_n27 };
    }
    struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_conditional_managed_move_result_n65 = hxc_l_tmp_conditional_managed_result_n24;
    hxc_l_tmp_instance_call_argument_1_owner_n28 = hxc_l_tmp_conditional_managed_move_result_n65;
    struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_borrow_result_n66 = hxc_l_tmp_instance_call_argument_1_owner_n28;
    struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_tmp_instance_call_receiver_load_result_n67 = hxc_l_tmp_instance_call_receiver_n21;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_instance_call_argument_0_load_result_n68 = hxc_l_tmp_instance_call_argument_0_n22;
    hxc_l_gc_roots[30] = (const void *)hxc_l_tmp_instance_call_argument_0_load_result_n68;
    if (hxc_l_tmp_instance_call_receiver_load_result_n67 == NULL)
    {
      abort();
    }
    hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_tmp_instance_call_receiver_load_result_n67, hxc_l_tmp_instance_call_argument_0_load_result_n68, hxc_l_tmp_instance_call_argument_1_borrow_result_n66);
    hxc_l_gc_roots[31] = (const void *)NULL;
    hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n28);
    if (hxc_string_release(&hxc_l_id) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_instance_call_argument_1_owner_n17) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_array_ref_release(hxc_l_tmp_instance_call_argument_2_owner_n13) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_path) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n11) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_right_owner_n10) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return NULL;
  }
  hxc_l_gc_roots[12] = (const void *)hxc_l_result;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_result;
}

hxc_string hxc_caxecraft_localization_RuntimeUiCatalog_readText(struct hxc_caxecraft_content_RuntimeSchemaReader *hxc_l_reader, struct hxc_caxecraft_content_ContentJsonNode *hxc_l_node, hxc_string hxc_l_path)
{
  const void *volatile hxc_l_gc_roots[7] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_node;
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
  const void *volatile hxc_l_gc_roots[15] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_node;
  hxc_l_gc_roots[1] = (const void *)hxc_l_fields;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 15, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_locale_hab6a4c8bb9fd = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n12 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n14 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_2_owner_n15 = { 0 };
  struct hxc_array_ref *hxc_l_result = { 0 };
  hxc_string hxc_l_locale_ha18eaa12adad = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n19 = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n24 = { 0 };
  struct hxc_caxecraft_content_RuntimeSchemaErrorKind hxc_l_tmp_instance_call_argument_1_owner_n25 = { 0 };
  hxc_string hxc_l_text = { 0 };
  hxc_string hxc_l_tmp_string_concat_left_owner_n27 = { 0 };
  hxc_string hxc_l_tmp_static_call_argument_2_owner_n28 = { 0 };
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
      bool hxc_l_tmp_string_equality_result_n13 = hxc_l_tmp_class_field_load_result_n17.data == NULL || hxc_l_locale_hab6a4c8bb9fd.data == NULL ? hxc_l_tmp_class_field_load_result_n17.data == hxc_l_locale_hab6a4c8bb9fd.data : hxc_l_tmp_class_field_load_result_n17.byte_length == hxc_l_locale_hab6a4c8bb9fd.byte_length && (hxc_l_tmp_class_field_load_result_n17.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n17.data, hxc_l_locale_hab6a4c8bb9fd.data, hxc_l_tmp_class_field_load_result_n17.byte_length) == 0);
      if (hxc_l_tmp_string_equality_result_n13)
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
      struct hxc_caxecraft_content_ContentJsonField *hxc_l_tmp_load_result_n22 = hxc_l_field_h26b10096402b;
      hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n22;
      if (hxc_l_tmp_load_result_n22 == NULL)
      {
        abort();
      }
      int32_t hxc_l_tmp_class_field_load_result_n23 = (*hxc_l_tmp_load_result_n22).hxc_line;
      struct hxc_caxecraft_content_ContentJsonField *hxc_l_tmp_load_result_n24 = hxc_l_field_h26b10096402b;
      hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_load_result_n24;
      if (hxc_l_tmp_load_result_n24 == NULL)
      {
        abort();
      }
      int32_t hxc_l_tmp_class_field_load_result_n25 = (*hxc_l_tmp_load_result_n24).hxc_column;
      hxc_string hxc_l_tmp_string_concat_result_n26 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n26) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_string_concat_left_owner_n14 = hxc_l_tmp_string_concat_result_n26;
      hxc_string hxc_l_tmp_string_concat_left_borrow_result_n27 = hxc_l_tmp_string_concat_left_owner_n14;
      struct hxc_caxecraft_content_ContentJsonField *hxc_l_tmp_load_result_n28 = hxc_l_field_h26b10096402b;
      hxc_l_gc_roots[6] = (const void *)hxc_l_tmp_load_result_n28;
      if (hxc_l_tmp_load_result_n28 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_class_field_load_result_n29 = (*hxc_l_tmp_load_result_n28).hxc_name;
      hxc_string hxc_l_tmp_string_concat_result_n30 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_borrow_result_n27, hxc_l_tmp_class_field_load_result_n29, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n30) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_instance_call_argument_2_owner_n15 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidLocale, .hxc_payload.hxc_SchemaInvalidLocale.hxc_path = hxc_l_tmp_string_concat_result_n30 };
      hxc_caxecraft_content_RuntimeSchemaReader_rejectAt(hxc_l_reader, hxc_l_tmp_class_field_load_result_n23, hxc_l_tmp_class_field_load_result_n25, hxc_l_tmp_instance_call_argument_2_owner_n15);
      hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_2_owner_n15);
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n14) != HXC_STATUS_OK)
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
  struct hxc_array_ref *hxc_l_tmp_array_create_result_n33 = NULL;
  if (hxc_array_ref_create(hxc_default_allocator(), (hxc_array_element_ops){ sizeof(hxc_string), _Alignof(hxc_string), NULL, hxc_array_87af1a2c_element_copy, hxc_array_87af1a2c_element_assign, hxc_array_87af1a2c_element_destroy }, &hxc_l_tmp_array_create_result_n33) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_result = hxc_l_tmp_array_create_result_n33;
  int32_t hxc_l_g_h4eedb77a2851 = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n34 = hxc_l_g_h4eedb77a2851;
    int32_t hxc_l_tmp_array_length_result_n35;
    if (hxc_array_ref_length(hxc_l_locales, &hxc_l_tmp_array_length_result_n35) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n34 < hxc_l_tmp_array_length_result_n35))
    {
      break;
    }
    hxc_string hxc_l_tmp_array_get_result_n37;
    if (hxc_array_ref_get_copy(hxc_l_locales, (size_t)hxc_l_g_h4eedb77a2851, &hxc_l_tmp_array_get_result_n37) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n19 = hxc_l_tmp_array_get_result_n37;
    hxc_l_locale_ha18eaa12adad = hxc_l_tmp_array_element_owner_n19;
    if (hxc_string_retain(hxc_l_locale_ha18eaa12adad) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_g_h4eedb77a2851 = hxc_i32_add_wrapping(hxc_l_g_h4eedb77a2851, 1);
    hxc_l_gc_roots[7] = (const void *)NULL;
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_found = NULL;
    int32_t hxc_l_g_hf858a378b697 = 0;
    while (1)
    {
      int32_t hxc_l_tmp_load_result_n40 = hxc_l_g_hf858a378b697;
      int32_t hxc_l_tmp_array_length_result_n41;
      if (hxc_array_ref_length(hxc_l_fields, &hxc_l_tmp_array_length_result_n41) != HXC_STATUS_OK)
      {
        abort();
      }
      if (!(hxc_l_tmp_load_result_n40 < hxc_l_tmp_array_length_result_n41))
      {
        break;
      }
      struct hxc_caxecraft_content_ContentJsonField *hxc_l_tmp_array_get_result_n43;
      if (hxc_array_ref_get_copy(hxc_l_fields, (size_t)hxc_l_g_hf858a378b697, &hxc_l_tmp_array_get_result_n43) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_gc_roots[8] = (const void *)hxc_l_tmp_array_get_result_n43;
      struct hxc_caxecraft_content_ContentJsonField *hxc_l_field_h17689a866110 = hxc_l_tmp_array_get_result_n43;
      hxc_l_g_hf858a378b697 = hxc_i32_add_wrapping(hxc_l_g_hf858a378b697, 1);
      struct hxc_caxecraft_content_ContentJsonField *hxc_l_tmp_load_result_n45 = hxc_l_field_h17689a866110;
      hxc_l_gc_roots[9] = (const void *)hxc_l_tmp_load_result_n45;
      if (hxc_l_tmp_load_result_n45 == NULL)
      {
        abort();
      }
      hxc_string hxc_l_tmp_class_field_load_result_n46 = (*hxc_l_tmp_load_result_n45).hxc_name;
      bool hxc_l_tmp_string_equality_result_n23 = hxc_l_tmp_class_field_load_result_n46.data == NULL || hxc_l_locale_ha18eaa12adad.data == NULL ? hxc_l_tmp_class_field_load_result_n46.data == hxc_l_locale_ha18eaa12adad.data : hxc_l_tmp_class_field_load_result_n46.byte_length == hxc_l_locale_ha18eaa12adad.byte_length && (hxc_l_tmp_class_field_load_result_n46.byte_length == 0 || memcmp(hxc_l_tmp_class_field_load_result_n46.data, hxc_l_locale_ha18eaa12adad.data, hxc_l_tmp_class_field_load_result_n46.byte_length) == 0);
      if (hxc_l_tmp_string_equality_result_n23)
      {
        struct hxc_caxecraft_content_ContentJsonField *hxc_l_tmp_load_result_n50 = hxc_l_field_h17689a866110;
        hxc_l_gc_roots[12] = (const void *)hxc_l_tmp_load_result_n50;
        if (hxc_l_tmp_load_result_n50 == NULL)
        {
          abort();
        }
        struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_class_field_load_result_n51 = (*hxc_l_tmp_load_result_n50).hxc_value;
        hxc_l_gc_roots[13] = (const void *)hxc_l_tmp_class_field_load_result_n51;
        hxc_l_found = hxc_l_tmp_class_field_load_result_n51;
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
      hxc_string hxc_l_tmp_string_concat_result_n53 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n53) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_string_concat_left_owner_n24 = hxc_l_tmp_string_concat_result_n53;
      hxc_string hxc_l_tmp_string_concat_left_borrow_result_n54 = hxc_l_tmp_string_concat_left_owner_n24;
      hxc_string hxc_l_tmp_string_concat_result_n56 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_borrow_result_n54, hxc_l_locale_ha18eaa12adad, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n56) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_tmp_instance_call_argument_1_owner_n25 = (struct hxc_caxecraft_content_RuntimeSchemaErrorKind){ .hxc_tag = hxc_caxecraft_content_RuntimeSchemaErrorKind_SchemaInvalidLocale, .hxc_payload.hxc_SchemaInvalidLocale.hxc_path = hxc_l_tmp_string_concat_result_n56 };
      hxc_caxecraft_content_RuntimeSchemaReader_reject(hxc_l_reader, hxc_l_node, hxc_l_tmp_instance_call_argument_1_owner_n25);
      hxc_enum_7ebbe54d_destroy(&hxc_l_tmp_instance_call_argument_1_owner_n25);
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n24) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_locale_ha18eaa12adad) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_array_element_owner_n19) != HXC_STATUS_OK)
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
    struct hxc_caxecraft_content_ContentJsonNode *hxc_l_tmp_load_result_n59 = hxc_l_found;
    hxc_l_gc_roots[14] = (const void *)hxc_l_tmp_load_result_n59;
    hxc_string hxc_l_tmp_string_concat_result_n60 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_path, (hxc_string){ (const uint8_t *)".", 1, true, NULL }, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n60) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_string_concat_left_owner_n27 = hxc_l_tmp_string_concat_result_n60;
    hxc_string hxc_l_tmp_string_concat_left_borrow_result_n61 = hxc_l_tmp_string_concat_left_owner_n27;
    hxc_string hxc_l_tmp_string_concat_result_n63 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
    if (hxc_string_concat_ref(hxc_l_tmp_string_concat_left_borrow_result_n61, hxc_l_locale_ha18eaa12adad, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n63) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_static_call_argument_2_owner_n28 = hxc_l_tmp_string_concat_result_n63;
    hxc_string hxc_l_tmp_call_result_n65 = hxc_caxecraft_localization_RuntimeUiCatalog_readText(hxc_l_reader, hxc_l_tmp_load_result_n59, hxc_l_tmp_static_call_argument_2_owner_n28);
    hxc_l_text = hxc_l_tmp_call_result_n65;
    bool hxc_l_tmp_string_null_equality_result_n29 = hxc_l_text.data == NULL;
    if (hxc_l_tmp_string_null_equality_result_n29)
    {
      if (hxc_string_release(&hxc_l_text) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n28) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n27) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_locale_ha18eaa12adad) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_array_element_owner_n19) != HXC_STATUS_OK)
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
    struct hxc_array_ref *hxc_l_tmp_load_result_n68 = hxc_l_result;
    int32_t hxc_l_tmp_array_push_result_n70;
    if (hxc_array_ref_push_copy(hxc_l_tmp_load_result_n68, &hxc_l_text, &hxc_l_tmp_array_push_result_n70) != HXC_STATUS_OK)
    {
      abort();
    }
    (void)hxc_l_tmp_array_push_result_n70;
    if (hxc_string_release(&hxc_l_text) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_static_call_argument_2_owner_n28) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_string_concat_left_owner_n27) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_locale_ha18eaa12adad) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_string_release(&hxc_l_tmp_array_element_owner_n19) != HXC_STATUS_OK)
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
  const void *volatile hxc_l_gc_roots[9] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_node;
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
  const void *volatile hxc_l_gc_roots[2] = { NULL, NULL };
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

hxc_string hxc_caxecraft_localization_RuntimeUiCatalog_replacePlaceholder(hxc_string hxc_l_text, int32_t hxc_l_slot, hxc_string hxc_l_value)
{
  hxc_string hxc_l_tmp_compound_string_left_owner_n17 = { 0 };
  hxc_string hxc_l_tmp_compound_string_left_owner_n18 = { 0 };
  hxc_string hxc_l_tmp_compound_string_right_owner_n20 = { 0 };
  hxc_string hxc_l_result = (hxc_string){ (const uint8_t *)"", 0, true, NULL };
  if (hxc_string_retain(hxc_l_result) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_index = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n0 = hxc_l_index;
    if (hxc_l_text.data == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_string_length_result_n1;
    if (hxc_string_haxe_length(hxc_l_text, &hxc_l_tmp_string_length_result_n1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n0 < hxc_l_tmp_string_length_result_n1))
    {
      break;
    }
    int32_t hxc_l_tmp_load_result_n2 = hxc_l_index;
    int32_t hxc_l_tmp_string_length_result_n3;
    if (hxc_string_haxe_length(hxc_l_text, &hxc_l_tmp_string_length_result_n3) != HXC_STATUS_OK)
    {
      abort();
    }
    bool hxc_l_tmp_short_circuit_result_n5 = hxc_i32_add_wrapping(hxc_l_tmp_load_result_n2, 2) < hxc_l_tmp_string_length_result_n3;
    if (hxc_i32_add_wrapping(hxc_l_tmp_load_result_n2, 2) < hxc_l_tmp_string_length_result_n3)
    {
      struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_string_char_code_at_result_n5 = (struct hxc_optional_value_h76f8e80e9d2b){ .hxc_has_value = false };
      hxc_l_tmp_string_char_code_at_result_n5.hxc_has_value = hxc_string_char_code_at(hxc_l_text, hxc_l_index, &hxc_l_tmp_string_char_code_at_result_n5.hxc_value);
      struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_optional_scalar_equality_optional_n6 = hxc_l_tmp_string_char_code_at_result_n5;
      int32_t hxc_l_tmp_optional_scalar_equality_scalar_n7 = 123;
      bool hxc_l_tmp_optional_scalar_equality_result_n8 = false;
      if (hxc_l_tmp_optional_scalar_equality_optional_n6.hxc_has_value)
      {
        struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_optional_scalar_compare_optional_load_result_n7 = hxc_l_tmp_optional_scalar_equality_optional_n6;
        int32_t hxc_l_tmp_optional_scalar_compare_scalar_load_result_n8 = hxc_l_tmp_optional_scalar_equality_scalar_n7;
        if (!hxc_l_tmp_optional_scalar_compare_optional_load_result_n7.hxc_has_value)
        {
          abort();
        }
        hxc_l_tmp_optional_scalar_equality_result_n8 = hxc_l_tmp_optional_scalar_compare_optional_load_result_n7.hxc_value == hxc_l_tmp_optional_scalar_compare_scalar_load_result_n8;
      }
      hxc_l_tmp_short_circuit_result_n5 = hxc_l_tmp_optional_scalar_equality_result_n8;
    }
    bool hxc_l_tmp_short_circuit_load_result_n10 = hxc_l_tmp_short_circuit_result_n5;
    bool hxc_l_tmp_short_circuit_result_n9 = hxc_l_tmp_short_circuit_load_result_n10;
    if (hxc_l_tmp_short_circuit_load_result_n10)
    {
      struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_string_char_code_at_result_n12 = (struct hxc_optional_value_h76f8e80e9d2b){ .hxc_has_value = false };
      hxc_l_tmp_string_char_code_at_result_n12.hxc_has_value = hxc_string_char_code_at(hxc_l_text, hxc_i32_add_wrapping(hxc_l_index, 1), &hxc_l_tmp_string_char_code_at_result_n12.hxc_value);
      struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_optional_scalar_equality_optional_n10 = hxc_l_tmp_string_char_code_at_result_n12;
      int32_t hxc_l_tmp_optional_scalar_equality_scalar_n11 = hxc_i32_add_wrapping(48, hxc_l_slot);
      bool hxc_l_tmp_optional_scalar_equality_result_n12 = false;
      if (hxc_l_tmp_optional_scalar_equality_optional_n10.hxc_has_value)
      {
        struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_optional_scalar_compare_optional_load_result_n14 = hxc_l_tmp_optional_scalar_equality_optional_n10;
        int32_t hxc_l_tmp_optional_scalar_compare_scalar_load_result_n15 = hxc_l_tmp_optional_scalar_equality_scalar_n11;
        if (!hxc_l_tmp_optional_scalar_compare_optional_load_result_n14.hxc_has_value)
        {
          abort();
        }
        hxc_l_tmp_optional_scalar_equality_result_n12 = hxc_l_tmp_optional_scalar_compare_optional_load_result_n14.hxc_value == hxc_l_tmp_optional_scalar_compare_scalar_load_result_n15;
      }
      hxc_l_tmp_short_circuit_result_n9 = hxc_l_tmp_optional_scalar_equality_result_n12;
    }
    bool hxc_l_tmp_short_circuit_load_result_n17 = hxc_l_tmp_short_circuit_result_n9;
    bool hxc_l_tmp_short_circuit_result_n13 = hxc_l_tmp_short_circuit_load_result_n17;
    if (hxc_l_tmp_short_circuit_load_result_n17)
    {
      struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_string_char_code_at_result_n19 = (struct hxc_optional_value_h76f8e80e9d2b){ .hxc_has_value = false };
      hxc_l_tmp_string_char_code_at_result_n19.hxc_has_value = hxc_string_char_code_at(hxc_l_text, hxc_i32_add_wrapping(hxc_l_index, 2), &hxc_l_tmp_string_char_code_at_result_n19.hxc_value);
      struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_optional_scalar_equality_optional_n14 = hxc_l_tmp_string_char_code_at_result_n19;
      int32_t hxc_l_tmp_optional_scalar_equality_scalar_n15 = 125;
      bool hxc_l_tmp_optional_scalar_equality_result_n16 = false;
      if (hxc_l_tmp_optional_scalar_equality_optional_n14.hxc_has_value)
      {
        struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_optional_scalar_compare_optional_load_result_n21 = hxc_l_tmp_optional_scalar_equality_optional_n14;
        int32_t hxc_l_tmp_optional_scalar_compare_scalar_load_result_n22 = hxc_l_tmp_optional_scalar_equality_scalar_n15;
        if (!hxc_l_tmp_optional_scalar_compare_optional_load_result_n21.hxc_has_value)
        {
          abort();
        }
        hxc_l_tmp_optional_scalar_equality_result_n16 = hxc_l_tmp_optional_scalar_compare_optional_load_result_n21.hxc_value == hxc_l_tmp_optional_scalar_compare_scalar_load_result_n22;
      }
      hxc_l_tmp_short_circuit_result_n13 = hxc_l_tmp_optional_scalar_equality_result_n16;
    }
    if (hxc_l_tmp_short_circuit_result_n13)
    {
      hxc_l_tmp_compound_string_left_owner_n17 = hxc_l_result;
      if (hxc_string_retain(hxc_l_tmp_compound_string_left_owner_n17) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_string hxc_l_tmp_string_concat_result_n27 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_tmp_compound_string_left_owner_n17, hxc_l_value, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n27) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_result) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_result = hxc_l_tmp_string_concat_result_n27;
      hxc_l_index = hxc_i32_add_wrapping(hxc_l_index, 3);
      if (hxc_string_release(&hxc_l_tmp_compound_string_left_owner_n17) != HXC_STATUS_OK)
      {
        abort();
      }
    }
    else
    {
      hxc_l_tmp_compound_string_left_owner_n18 = hxc_l_result;
      if (hxc_string_retain(hxc_l_tmp_compound_string_left_owner_n18) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_string hxc_l_tmp_compound_string_left_borrow_result_n30 = hxc_l_tmp_compound_string_left_owner_n18;
      hxc_string hxc_l_tmp_string_char_at_result_n32 = hxc_string_char_at(hxc_l_text, hxc_l_index);
      hxc_string hxc_l_tmp_string_char_at_owner_n19 = { 0 };
      hxc_l_tmp_string_char_at_owner_n19 = hxc_l_tmp_string_char_at_result_n32;
      if (hxc_string_retain(hxc_l_tmp_string_char_at_owner_n19) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_string hxc_l_tmp_string_char_at_owned_result_n33 = hxc_l_tmp_string_char_at_owner_n19;
      hxc_l_tmp_compound_string_right_owner_n20 = hxc_l_tmp_string_char_at_owned_result_n33;
      hxc_string hxc_l_tmp_string_concat_result_n35 = (hxc_string){ .data = NULL, .byte_length = 0U, .has_trailing_nul = false, .owner = NULL };
      if (hxc_string_concat_ref(hxc_l_tmp_compound_string_left_borrow_result_n30, hxc_l_tmp_compound_string_right_owner_n20, hxc_default_allocator(), &hxc_l_tmp_string_concat_result_n35) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_result) != HXC_STATUS_OK)
      {
        abort();
      }
      hxc_l_result = hxc_l_tmp_string_concat_result_n35;
      hxc_l_index = hxc_i32_add_wrapping(hxc_l_index, 1);
      if (hxc_string_release(&hxc_l_tmp_compound_string_right_owner_n20) != HXC_STATUS_OK)
      {
        abort();
      }
      if (hxc_string_release(&hxc_l_tmp_compound_string_left_owner_n18) != HXC_STATUS_OK)
      {
        abort();
      }
    }
  }
  return hxc_l_result;
}

bool hxc_caxecraft_localization_RuntimeUiCatalog_validPlaceholderTokens(hxc_string hxc_l_text)
{
  int32_t hxc_l_index = 0;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n0 = hxc_l_index;
    if (hxc_l_text.data == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_string_length_result_n1;
    if (hxc_string_haxe_length(hxc_l_text, &hxc_l_tmp_string_length_result_n1) != HXC_STATUS_OK)
    {
      abort();
    }
    if (!(hxc_l_tmp_load_result_n0 < hxc_l_tmp_string_length_result_n1))
    {
      break;
    }
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_string_char_code_at_result_n3 = (struct hxc_optional_value_h76f8e80e9d2b){ .hxc_has_value = false };
    hxc_l_tmp_string_char_code_at_result_n3.hxc_has_value = hxc_string_char_code_at(hxc_l_text, hxc_l_index, &hxc_l_tmp_string_char_code_at_result_n3.hxc_value);
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_code = hxc_l_tmp_string_char_code_at_result_n3;
    struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_optional_scalar_equality_optional_n3 = hxc_l_code;
    int32_t hxc_l_tmp_optional_scalar_equality_scalar_n4 = 123;
    bool hxc_l_tmp_optional_scalar_equality_result_n5 = false;
    if (hxc_l_tmp_optional_scalar_equality_optional_n3.hxc_has_value)
    {
      struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_optional_scalar_compare_optional_load_result_n6 = hxc_l_tmp_optional_scalar_equality_optional_n3;
      int32_t hxc_l_tmp_optional_scalar_compare_scalar_load_result_n7 = hxc_l_tmp_optional_scalar_equality_scalar_n4;
      if (!hxc_l_tmp_optional_scalar_compare_optional_load_result_n6.hxc_has_value)
      {
        abort();
      }
      hxc_l_tmp_optional_scalar_equality_result_n5 = hxc_l_tmp_optional_scalar_compare_optional_load_result_n6.hxc_value == hxc_l_tmp_optional_scalar_compare_scalar_load_result_n7;
    }
    if (hxc_l_tmp_optional_scalar_equality_result_n5)
    {
      int32_t hxc_l_tmp_load_result_n9 = hxc_l_index;
      int32_t hxc_l_tmp_string_length_result_n10;
      if (hxc_string_haxe_length(hxc_l_text, &hxc_l_tmp_string_length_result_n10) != HXC_STATUS_OK)
      {
        abort();
      }
      bool hxc_l_tmp_short_circuit_result_n6 = hxc_i32_add_wrapping(hxc_l_tmp_load_result_n9, 2) >= hxc_l_tmp_string_length_result_n10;
      if (!(hxc_i32_add_wrapping(hxc_l_tmp_load_result_n9, 2) >= hxc_l_tmp_string_length_result_n10))
      {
        struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_string_char_code_at_result_n12 = (struct hxc_optional_value_h76f8e80e9d2b){ .hxc_has_value = false };
        hxc_l_tmp_string_char_code_at_result_n12.hxc_has_value = hxc_string_char_code_at(hxc_l_text, hxc_i32_add_wrapping(hxc_l_index, 1), &hxc_l_tmp_string_char_code_at_result_n12.hxc_value);
        if (!hxc_l_tmp_string_char_code_at_result_n12.hxc_has_value)
        {
          abort();
        }
        hxc_l_tmp_short_circuit_result_n6 = hxc_l_tmp_string_char_code_at_result_n12.hxc_value < 48;
      }
      bool hxc_l_tmp_short_circuit_load_result_n14 = hxc_l_tmp_short_circuit_result_n6;
      bool hxc_l_tmp_short_circuit_result_n7 = hxc_l_tmp_short_circuit_load_result_n14;
      if (!hxc_l_tmp_short_circuit_load_result_n14)
      {
        struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_string_char_code_at_result_n16 = (struct hxc_optional_value_h76f8e80e9d2b){ .hxc_has_value = false };
        hxc_l_tmp_string_char_code_at_result_n16.hxc_has_value = hxc_string_char_code_at(hxc_l_text, hxc_i32_add_wrapping(hxc_l_index, 1), &hxc_l_tmp_string_char_code_at_result_n16.hxc_value);
        if (!hxc_l_tmp_string_char_code_at_result_n16.hxc_has_value)
        {
          abort();
        }
        hxc_l_tmp_short_circuit_result_n7 = hxc_l_tmp_string_char_code_at_result_n16.hxc_value > 57;
      }
      bool hxc_l_tmp_short_circuit_load_result_n18 = hxc_l_tmp_short_circuit_result_n7;
      bool hxc_l_tmp_short_circuit_result_n8 = hxc_l_tmp_short_circuit_load_result_n18;
      if (!hxc_l_tmp_short_circuit_load_result_n18)
      {
        struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_string_char_code_at_result_n20 = (struct hxc_optional_value_h76f8e80e9d2b){ .hxc_has_value = false };
        hxc_l_tmp_string_char_code_at_result_n20.hxc_has_value = hxc_string_char_code_at(hxc_l_text, hxc_i32_add_wrapping(hxc_l_index, 2), &hxc_l_tmp_string_char_code_at_result_n20.hxc_value);
        struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_optional_scalar_equality_optional_n9 = hxc_l_tmp_string_char_code_at_result_n20;
        int32_t hxc_l_tmp_optional_scalar_equality_scalar_n10 = 125;
        bool hxc_l_tmp_optional_scalar_equality_result_n11 = true;
        if (hxc_l_tmp_optional_scalar_equality_optional_n9.hxc_has_value)
        {
          struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_optional_scalar_compare_optional_load_result_n22 = hxc_l_tmp_optional_scalar_equality_optional_n9;
          int32_t hxc_l_tmp_optional_scalar_compare_scalar_load_result_n23 = hxc_l_tmp_optional_scalar_equality_scalar_n10;
          if (!hxc_l_tmp_optional_scalar_compare_optional_load_result_n22.hxc_has_value)
          {
            abort();
          }
          hxc_l_tmp_optional_scalar_equality_result_n11 = hxc_l_tmp_optional_scalar_compare_optional_load_result_n22.hxc_value != hxc_l_tmp_optional_scalar_compare_scalar_load_result_n23;
        }
        hxc_l_tmp_short_circuit_result_n8 = hxc_l_tmp_optional_scalar_equality_result_n11;
      }
      if (hxc_l_tmp_short_circuit_result_n8)
      {
        return false;
      }
      else
      {
        hxc_l_index = hxc_i32_add_wrapping(hxc_l_index, 3);
      }
    }
    else
    {
      struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_optional_scalar_equality_optional_n12 = hxc_l_code;
      int32_t hxc_l_tmp_optional_scalar_equality_scalar_n13 = 125;
      bool hxc_l_tmp_optional_scalar_equality_result_n14 = false;
      if (hxc_l_tmp_optional_scalar_equality_optional_n12.hxc_has_value)
      {
        struct hxc_optional_value_h76f8e80e9d2b hxc_l_tmp_optional_scalar_compare_optional_load_result_n29 = hxc_l_tmp_optional_scalar_equality_optional_n12;
        int32_t hxc_l_tmp_optional_scalar_compare_scalar_load_result_n30 = hxc_l_tmp_optional_scalar_equality_scalar_n13;
        if (!hxc_l_tmp_optional_scalar_compare_optional_load_result_n29.hxc_has_value)
        {
          abort();
        }
        hxc_l_tmp_optional_scalar_equality_result_n14 = hxc_l_tmp_optional_scalar_compare_optional_load_result_n29.hxc_value == hxc_l_tmp_optional_scalar_compare_scalar_load_result_n30;
      }
      if (hxc_l_tmp_optional_scalar_equality_result_n14)
      {
        return false;
      }
      else
      {
        hxc_l_index = hxc_i32_add_wrapping(hxc_l_index, 1);
      }
    }
  }
  return true;
}

hxc_string hxc_caxecraft_localization_RuntimeUiCatalog_format(struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_self, int32_t hxc_l_locale, hxc_string hxc_l_message, struct hxc_array_ref *hxc_l_arguments)
{
  const void *volatile hxc_l_gc_roots[6] = { NULL, NULL, NULL, NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 6, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_result = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n11 = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n15 = { 0 };
  hxc_string hxc_l_tmp_returned_string_owner_n6 = { 0 };
  hxc_string hxc_l_tmp_returned_string_owner_n9 = { 0 };
  int32_t hxc_l_tmp_call_result_n0 = hxc_caxecraft_localization_RuntimeUiCatalog_localeStorageCode(hxc_l_locale);
  int32_t hxc_l_localeCode = hxc_l_tmp_call_result_n0;
  int32_t hxc_l_tmp_load_result_n1 = hxc_l_localeCode;
  bool hxc_l_tmp_short_circuit_result_n5 = hxc_l_tmp_load_result_n1 < 0;
  if (!(hxc_l_tmp_load_result_n1 < 0))
  {
    int32_t hxc_l_tmp_load_result_n2 = hxc_l_localeCode;
    if (hxc_l_self == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n3 = (*hxc_l_self).hxc_locales;
    int32_t hxc_l_tmp_array_length_result_n4;
    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n3, &hxc_l_tmp_array_length_result_n4) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n5 = hxc_l_tmp_load_result_n2 >= hxc_l_tmp_array_length_result_n4;
  }
  if (hxc_l_tmp_short_circuit_result_n5)
  {
    hxc_l_tmp_returned_string_owner_n6 = (hxc_string){ (const uint8_t *)"", 0, true, NULL };
    if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n6) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_returned_string_owner_n6;
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiTemplateDefinition *hxc_l_tmp_instance_call_result_n7 = hxc_caxecraft_localization_RuntimeUiCatalog_templateFor(hxc_l_self, hxc_l_message);
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_instance_call_result_n7;
  struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiTemplateDefinition *hxc_l_template = hxc_l_tmp_instance_call_result_n7;
  struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiTemplateDefinition *hxc_l_tmp_load_result_n8 = hxc_l_template;
  hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_load_result_n8;
  hxc_l_gc_roots[3] = (const void *)NULL;
  bool hxc_l_tmp_short_circuit_result_n8 = hxc_l_tmp_load_result_n8 == NULL;
  if (!(hxc_l_tmp_load_result_n8 == NULL))
  {
    int32_t hxc_l_tmp_array_length_result_n9;
    if (hxc_array_ref_length(hxc_l_arguments, &hxc_l_tmp_array_length_result_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiTemplateDefinition *hxc_l_tmp_load_result_n10 = hxc_l_template;
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n10;
    if (hxc_l_tmp_load_result_n10 == NULL)
    {
      abort();
    }
    int32_t hxc_l_tmp_class_field_load_result_n11 = (*hxc_l_tmp_load_result_n10).hxc_argumentCount;
    hxc_l_tmp_short_circuit_result_n8 = hxc_l_tmp_array_length_result_n9 != hxc_l_tmp_class_field_load_result_n11;
  }
  if (hxc_l_tmp_short_circuit_result_n8)
  {
    hxc_l_tmp_returned_string_owner_n9 = (hxc_string){ (const uint8_t *)"", 0, true, NULL };
    if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_returned_string_owner_n9;
  }
  struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiTemplateDefinition *hxc_l_tmp_load_result_n14 = hxc_l_template;
  hxc_l_gc_roots[5] = (const void *)hxc_l_tmp_load_result_n14;
  if (hxc_l_tmp_load_result_n14 == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n15 = (*hxc_l_tmp_load_result_n14).hxc_texts;
  hxc_string hxc_l_tmp_array_get_result_n17;
  if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n15, (size_t)hxc_l_localeCode, &hxc_l_tmp_array_get_result_n17) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_l_tmp_array_element_owner_n11 = hxc_l_tmp_array_get_result_n17;
  hxc_l_result = hxc_l_tmp_array_element_owner_n11;
  if (hxc_string_retain(hxc_l_result) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g = 0;
  int32_t hxc_l_tmp_array_length_result_n19;
  if (hxc_array_ref_length(hxc_l_arguments, &hxc_l_tmp_array_length_result_n19) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_g1 = hxc_l_tmp_array_length_result_n19;
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n20 = hxc_l_g;
    if (!(hxc_l_tmp_load_result_n20 < hxc_l_g1))
    {
      break;
    }
    int32_t hxc_l_tmp_increment_load_result_n22 = hxc_l_g;
    hxc_l_g = hxc_i32_add_wrapping(hxc_l_tmp_increment_load_result_n22, 1);
    int32_t hxc_l_index = hxc_l_tmp_increment_load_result_n22;
    hxc_string hxc_l_tmp_load_result_n23 = hxc_l_result;
    int32_t hxc_l_tmp_load_result_n24 = hxc_l_index;
    hxc_string hxc_l_tmp_array_get_result_n26;
    if (hxc_array_ref_get_copy(hxc_l_arguments, (size_t)hxc_l_index, &hxc_l_tmp_array_get_result_n26) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n15 = hxc_l_tmp_array_get_result_n26;
    hxc_string hxc_l_tmp_call_result_n28 = hxc_caxecraft_localization_RuntimeUiCatalog_replacePlaceholder(hxc_l_tmp_load_result_n23, hxc_l_tmp_load_result_n24, hxc_l_tmp_array_element_owner_n15);
    if (hxc_string_release(&hxc_l_result) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_result = hxc_l_tmp_call_result_n28;
    if (hxc_string_release(&hxc_l_tmp_array_element_owner_n15) != HXC_STATUS_OK)
    {
      abort();
    }
  }
  hxc_string hxc_l_tmp_load_result_n29 = hxc_l_result;
  if (hxc_string_release(&hxc_l_tmp_array_element_owner_n11) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_load_result_n29;
}

struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *hxc_caxecraft_localization_RuntimeUiCatalog_messageFor(struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_self, hxc_string hxc_l_expected)
{
  const void *volatile hxc_l_gc_roots[7] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 7, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_low = 0;
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_messages;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  int32_t hxc_l_tmp_array_length_result_n1;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n0, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_high = hxc_i32_subtract_wrapping(hxc_l_tmp_array_length_result_n1, 1);
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n2 = hxc_l_low;
    if (!(hxc_l_tmp_load_result_n2 <= hxc_l_high))
    {
      break;
    }
    int32_t hxc_l_tmp_load_result_n4 = hxc_l_low;
    int32_t hxc_l_tmp_load_result_n5 = hxc_l_high;
    int32_t hxc_l_middle = hxc_i32_add_wrapping(hxc_l_tmp_load_result_n4, hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n5, hxc_l_low) / 2);
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n7 = (*hxc_l_self).hxc_messages;
    hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_class_field_load_result_n7;
    struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *hxc_l_tmp_array_get_result_n9;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n7, (size_t)hxc_l_middle, &hxc_l_tmp_array_get_result_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_array_get_result_n9;
    struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *hxc_l_candidate = hxc_l_tmp_array_get_result_n9;
    struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *hxc_l_tmp_load_result_n10 = hxc_l_candidate;
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n10;
    if (hxc_l_tmp_load_result_n10 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n11 = (*hxc_l_tmp_load_result_n10).hxc_id;
    int32_t hxc_l_tmp_call_result_n12 = hxc_caxecraft_content_RuntimeSchemaReader_compareUtf8(hxc_l_tmp_class_field_load_result_n11, hxc_l_expected);
    int32_t hxc_l_ordering = hxc_l_tmp_call_result_n12;
    if (hxc_l_ordering == 0)
    {
      hxc_l_gc_roots[6] = (const void *)hxc_l_candidate;
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_candidate;
    }
    if (hxc_l_ordering < 0)
    {
      hxc_l_low = hxc_i32_add_wrapping(hxc_l_middle, 1);
    }
    else
    {
      hxc_l_high = hxc_i32_subtract_wrapping(hxc_l_middle, 1);
    }
  }
  hxc_l_gc_roots[5] = (const void *)NULL;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return NULL;
}

int32_t hxc_caxecraft_localization_RuntimeUiCatalog_nextLocale(struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_self, int32_t hxc_l_locale)
{
  const void *volatile hxc_l_gc_roots[1] = { NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
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

struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiTemplateDefinition *hxc_caxecraft_localization_RuntimeUiCatalog_templateFor(struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_self, hxc_string hxc_l_expected)
{
  const void *volatile hxc_l_gc_roots[7] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 7, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_low = 0;
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n0 = (*hxc_l_self).hxc_templates;
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_class_field_load_result_n0;
  int32_t hxc_l_tmp_array_length_result_n1;
  if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n0, &hxc_l_tmp_array_length_result_n1) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_l_high = hxc_i32_subtract_wrapping(hxc_l_tmp_array_length_result_n1, 1);
  while (1)
  {
    int32_t hxc_l_tmp_load_result_n2 = hxc_l_low;
    if (!(hxc_l_tmp_load_result_n2 <= hxc_l_high))
    {
      break;
    }
    int32_t hxc_l_tmp_load_result_n4 = hxc_l_low;
    int32_t hxc_l_tmp_load_result_n5 = hxc_l_high;
    int32_t hxc_l_middle = hxc_i32_add_wrapping(hxc_l_tmp_load_result_n4, hxc_i32_subtract_wrapping(hxc_l_tmp_load_result_n5, hxc_l_low) / 2);
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n7 = (*hxc_l_self).hxc_templates;
    hxc_l_gc_roots[2] = (const void *)hxc_l_tmp_class_field_load_result_n7;
    struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiTemplateDefinition *hxc_l_tmp_array_get_result_n9;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n7, (size_t)hxc_l_middle, &hxc_l_tmp_array_get_result_n9) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_gc_roots[3] = (const void *)hxc_l_tmp_array_get_result_n9;
    struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiTemplateDefinition *hxc_l_candidate = hxc_l_tmp_array_get_result_n9;
    struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiTemplateDefinition *hxc_l_tmp_load_result_n10 = hxc_l_candidate;
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n10;
    if (hxc_l_tmp_load_result_n10 == NULL)
    {
      abort();
    }
    hxc_string hxc_l_tmp_class_field_load_result_n11 = (*hxc_l_tmp_load_result_n10).hxc_id;
    int32_t hxc_l_tmp_call_result_n12 = hxc_caxecraft_content_RuntimeSchemaReader_compareUtf8(hxc_l_tmp_class_field_load_result_n11, hxc_l_expected);
    int32_t hxc_l_ordering = hxc_l_tmp_call_result_n12;
    if (hxc_l_ordering == 0)
    {
      hxc_l_gc_roots[6] = (const void *)hxc_l_candidate;
      if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
      {
        abort();
      }
      return hxc_l_candidate;
    }
    if (hxc_l_ordering < 0)
    {
      hxc_l_low = hxc_i32_add_wrapping(hxc_l_middle, 1);
    }
    else
    {
      hxc_l_high = hxc_i32_subtract_wrapping(hxc_l_middle, 1);
    }
  }
  hxc_l_gc_roots[5] = (const void *)NULL;
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return NULL;
}

hxc_string hxc_caxecraft_localization_RuntimeUiCatalog_text(struct hxc_caxecraft_localization_RuntimeUiCatalog *hxc_l_self, int32_t hxc_l_locale, hxc_string hxc_l_message)
{
  const void *volatile hxc_l_gc_roots[5] = { NULL, NULL, NULL, NULL, NULL };
  hxc_l_gc_roots[0] = (const void *)hxc_l_self;
  struct hxc_gc_root_frame hxc_l_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_l_gc_roots, 5, &hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_string hxc_l_tmp_returned_string_owner_n5 = { 0 };
  hxc_string hxc_l_tmp_array_element_owner_n8 = { 0 };
  int32_t hxc_l_tmp_call_result_n0 = hxc_caxecraft_localization_RuntimeUiCatalog_localeStorageCode(hxc_l_locale);
  int32_t hxc_l_localeCode = hxc_l_tmp_call_result_n0;
  int32_t hxc_l_tmp_load_result_n1 = hxc_l_localeCode;
  bool hxc_l_tmp_short_circuit_result_n4 = hxc_l_tmp_load_result_n1 < 0;
  if (!(hxc_l_tmp_load_result_n1 < 0))
  {
    int32_t hxc_l_tmp_load_result_n2 = hxc_l_localeCode;
    if (hxc_l_self == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n3 = (*hxc_l_self).hxc_locales;
    int32_t hxc_l_tmp_array_length_result_n4;
    if (hxc_array_ref_length(hxc_l_tmp_class_field_load_result_n3, &hxc_l_tmp_array_length_result_n4) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_short_circuit_result_n4 = hxc_l_tmp_load_result_n2 >= hxc_l_tmp_array_length_result_n4;
  }
  if (hxc_l_tmp_short_circuit_result_n4)
  {
    hxc_l_tmp_returned_string_owner_n5 = (hxc_string){ (const uint8_t *)"", 0, true, NULL };
    if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n5) != HXC_STATUS_OK)
    {
      abort();
    }
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_returned_string_owner_n5;
  }
  if (hxc_l_self == NULL)
  {
    abort();
  }
  struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *hxc_l_tmp_instance_call_result_n7 = hxc_caxecraft_localization_RuntimeUiCatalog_messageFor(hxc_l_self, hxc_l_message);
  hxc_l_gc_roots[1] = (const void *)hxc_l_tmp_instance_call_result_n7;
  struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *hxc_l_definition = hxc_l_tmp_instance_call_result_n7;
  hxc_l_gc_roots[2] = (const void *)hxc_l_definition;
  hxc_l_gc_roots[3] = (const void *)NULL;
  if (!(hxc_l_definition == NULL))
  {
    struct hxc_caxecraft_localization_RuntimeUiCatalog_RuntimeUiMessageDefinition *hxc_l_tmp_load_result_n10 = hxc_l_definition;
    hxc_l_gc_roots[4] = (const void *)hxc_l_tmp_load_result_n10;
    if (hxc_l_tmp_load_result_n10 == NULL)
    {
      abort();
    }
    struct hxc_array_ref *hxc_l_tmp_class_field_load_result_n11 = (*hxc_l_tmp_load_result_n10).hxc_texts;
    hxc_string hxc_l_tmp_array_get_result_n13;
    if (hxc_array_ref_get_copy(hxc_l_tmp_class_field_load_result_n11, (size_t)hxc_l_localeCode, &hxc_l_tmp_array_get_result_n13) != HXC_STATUS_OK)
    {
      abort();
    }
    hxc_l_tmp_array_element_owner_n8 = hxc_l_tmp_array_get_result_n13;
    if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
    {
      abort();
    }
    return hxc_l_tmp_array_element_owner_n8;
  }
  hxc_string hxc_l_tmp_returned_string_owner_n7 = (hxc_string){ (const uint8_t *)"", 0, true, NULL };
  if (hxc_string_retain(hxc_l_tmp_returned_string_owner_n7) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_l_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_l_tmp_returned_string_owner_n7;
}
