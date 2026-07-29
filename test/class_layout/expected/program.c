#include "hxc/program.h"

struct hxc_gc hxc_program_gc = HXC_GC_INITIALIZER;

struct hxc_gc_thread hxc_program_gc_thread = HXC_GC_THREAD_INITIALIZER;

_Static_assert(offsetof(struct hxc_RootRecord, hxc_rootValue) == 0, "class hxc_RootRecord first storage field begins at offset zero");

_Static_assert(_Alignof(struct hxc_RootRecord) >= _Alignof(int32_t), "class hxc_RootRecord alignment admits field 0");

_Static_assert(sizeof(struct hxc_RootRecord) >= offsetof(struct hxc_RootRecord, hxc_rootValue) + sizeof(int32_t), "class hxc_RootRecord size contains its final storage member");

_Static_assert(offsetof(struct hxc_MiddleRecord, hxc_base) == 0, "class hxc_MiddleRecord base subobject begins at offset zero");

_Static_assert(_Alignof(struct hxc_MiddleRecord) >= _Alignof(struct hxc_RootRecord), "class hxc_MiddleRecord alignment admits its base subobject");

_Static_assert(sizeof(struct hxc_MiddleRecord) >= sizeof(struct hxc_RootRecord), "class hxc_MiddleRecord contains its complete base subobject");

_Static_assert(offsetof(struct hxc_MiddleRecord, hxc_enabled) >= offsetof(struct hxc_MiddleRecord, hxc_base) + sizeof(struct hxc_RootRecord), "class hxc_MiddleRecord field 0 follows the prior storage without overlap");

_Static_assert(_Alignof(struct hxc_MiddleRecord) >= _Alignof(bool), "class hxc_MiddleRecord alignment admits field 0");

_Static_assert(sizeof(struct hxc_MiddleRecord) >= offsetof(struct hxc_MiddleRecord, hxc_enabled) + sizeof(bool), "class hxc_MiddleRecord size contains its final storage member");

_Static_assert(offsetof(struct hxc_LeafRecord, hxc_base) == 0, "class hxc_LeafRecord base subobject begins at offset zero");

_Static_assert(_Alignof(struct hxc_LeafRecord) >= _Alignof(struct hxc_MiddleRecord), "class hxc_LeafRecord alignment admits its base subobject");

_Static_assert(sizeof(struct hxc_LeafRecord) >= sizeof(struct hxc_MiddleRecord), "class hxc_LeafRecord contains its complete base subobject");

_Static_assert(offsetof(struct hxc_LeafRecord, hxc_score) >= offsetof(struct hxc_LeafRecord, hxc_base) + sizeof(struct hxc_MiddleRecord), "class hxc_LeafRecord field 0 follows the prior storage without overlap");

_Static_assert(_Alignof(struct hxc_LeafRecord) >= _Alignof(double), "class hxc_LeafRecord alignment admits field 0");

_Static_assert(offsetof(struct hxc_LeafRecord, hxc_peer) >= offsetof(struct hxc_LeafRecord, hxc_score) + sizeof(double), "class hxc_LeafRecord field 1 follows the prior storage without overlap");

_Static_assert(_Alignof(struct hxc_LeafRecord) >= _Alignof(struct hxc_RootRecord *), "class hxc_LeafRecord alignment admits field 1");

_Static_assert(sizeof(struct hxc_LeafRecord) >= offsetof(struct hxc_LeafRecord, hxc_peer) + sizeof(struct hxc_RootRecord *), "class hxc_LeafRecord size contains its final storage member");

_Static_assert(offsetof(struct hxc_EmptyBase, hxc_storage) == 0, "class hxc_EmptyBase strict-C empty-storage anchor begins at zero");

_Static_assert(sizeof(struct hxc_EmptyBase) >= 1, "class hxc_EmptyBase strict-C empty-storage anchor occupies one byte");

_Static_assert(offsetof(struct hxc_EmptyLeaf, hxc_base) == 0, "class hxc_EmptyLeaf base subobject begins at offset zero");

_Static_assert(_Alignof(struct hxc_EmptyLeaf) >= _Alignof(struct hxc_EmptyBase), "class hxc_EmptyLeaf alignment admits its base subobject");

_Static_assert(sizeof(struct hxc_EmptyLeaf) >= sizeof(struct hxc_EmptyBase), "class hxc_EmptyLeaf contains its complete base subobject");

_Static_assert(sizeof(struct hxc_EmptyLeaf) >= offsetof(struct hxc_EmptyLeaf, hxc_base) + sizeof(struct hxc_EmptyBase), "class hxc_EmptyLeaf size contains its final storage member");

static void hxc_LeafRecord_trace(const void *hxc_LeafRecord_trace_object, hxc_trace_visit_fn hxc_LeafRecord_trace_visit, void *hxc_LeafRecord_trace_context)
{
  const struct hxc_LeafRecord *hxc_LeafRecord_trace_typed = (const struct hxc_LeafRecord *)hxc_LeafRecord_trace_object;
  if ((*hxc_LeafRecord_trace_typed).hxc_peer != NULL)
  {
    hxc_LeafRecord_trace_visit(hxc_LeafRecord_trace_context, (*hxc_LeafRecord_trace_typed).hxc_peer);
  }
}

_Static_assert(sizeof(struct hxc_EmptyBase) % _Alignof(struct hxc_EmptyBase) == 0, "descriptor `class.2dd778e14cb710013ad030edefa961284618b305f3b865084d3bd57c341fb911` payload size must be a multiple of alignment");

const struct hxc_type_descriptor hxc_EmptyBase_descriptor = { .abi_version = HXC_TYPE_DESCRIPTOR_ABI_VERSION, .flags = 0, .object_size = sizeof(struct hxc_EmptyBase), .object_alignment = _Alignof(struct hxc_EmptyBase), .trace = NULL, .finalize = NULL };

_Static_assert(sizeof(struct hxc_EmptyLeaf) % _Alignof(struct hxc_EmptyLeaf) == 0, "descriptor `class.3e4c96cf3de0c1da0123f187164fd36d7d91a1b6cfd558e05b3c601bb034b93a` payload size must be a multiple of alignment");

const struct hxc_type_descriptor hxc_EmptyLeaf_descriptor = { .abi_version = HXC_TYPE_DESCRIPTOR_ABI_VERSION, .flags = 0, .object_size = sizeof(struct hxc_EmptyLeaf), .object_alignment = _Alignof(struct hxc_EmptyLeaf), .trace = NULL, .finalize = NULL };

_Static_assert(sizeof(struct hxc_MiddleRecord) % _Alignof(struct hxc_MiddleRecord) == 0, "descriptor `class.645466867fe98b7f467c7df7b91a1e41e066a6b82b1dc7d9fe75069a9fc98f53` payload size must be a multiple of alignment");

const struct hxc_type_descriptor hxc_MiddleRecord_descriptor = { .abi_version = HXC_TYPE_DESCRIPTOR_ABI_VERSION, .flags = 0, .object_size = sizeof(struct hxc_MiddleRecord), .object_alignment = _Alignof(struct hxc_MiddleRecord), .trace = NULL, .finalize = NULL };

_Static_assert(sizeof(struct hxc_LeafRecord) % _Alignof(struct hxc_LeafRecord) == 0, "descriptor `class.6496d5123c43d3e326fae55e002b27d083bb1e327a86985c522ebcded24489a6` payload size must be a multiple of alignment");

const struct hxc_type_descriptor hxc_LeafRecord_descriptor = { .abi_version = HXC_TYPE_DESCRIPTOR_ABI_VERSION, .flags = HXC_TYPE_DESCRIPTOR_HAS_TRACE, .object_size = sizeof(struct hxc_LeafRecord), .object_alignment = _Alignof(struct hxc_LeafRecord), .trace = hxc_LeafRecord_trace, .finalize = NULL };

_Static_assert(sizeof(struct hxc_RootRecord) % _Alignof(struct hxc_RootRecord) == 0, "descriptor `class.92fcac8eab337fdbc55ff736e8ef7a0f4cc6422987a431d2828483b341138634` payload size must be a multiple of alignment");

const struct hxc_type_descriptor hxc_RootRecord_descriptor = { .abi_version = HXC_TYPE_DESCRIPTOR_ABI_VERSION, .flags = 0, .object_size = sizeof(struct hxc_RootRecord), .object_alignment = _Alignof(struct hxc_RootRecord), .trace = NULL, .finalize = NULL };

struct hxc_EmptyBase *hxc_ClassLayoutFixture_asEmptyBase(struct hxc_EmptyLeaf *hxc_value)
{
  const void *hxc_gc_roots[2] = { (const void *)hxc_value, NULL };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 2, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_gc_roots[1] = (const void *)(hxc_value == NULL ? NULL : &hxc_value->hxc_base);
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_value == NULL ? NULL : &hxc_value->hxc_base;
}

struct hxc_MiddleRecord *hxc_ClassLayoutFixture_asMiddle(struct hxc_LeafRecord *hxc_value)
{
  const void *hxc_gc_roots[2] = { (const void *)hxc_value, NULL };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 2, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_gc_roots[1] = (const void *)(hxc_value == NULL ? NULL : &hxc_value->hxc_base);
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_value == NULL ? NULL : &hxc_value->hxc_base;
}

struct hxc_RootRecord *hxc_ClassLayoutFixture_asRoot(struct hxc_LeafRecord *hxc_value)
{
  const void *hxc_gc_roots[2] = { (const void *)hxc_value, NULL };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 2, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_gc_roots[1] = (const void *)(hxc_value == NULL ? NULL : &hxc_value->hxc_base.hxc_base);
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_value == NULL ? NULL : &hxc_value->hxc_base.hxc_base;
}

int32_t hxc_ClassLayoutFixture_branchProofDoesNotEscape(struct hxc_RootRecord *hxc_value, struct hxc_RootRecord *hxc_other, bool hxc_useFirst)
{
  const void *hxc_gc_roots[2] = { (const void *)hxc_value, (const void *)hxc_other };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 2, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  int32_t hxc_result = 0;
  if (hxc_useFirst)
  {
    if (hxc_value == NULL)
    {
      abort();
    }
    int32_t hxc_tmp_class_field_load_result_n0 = (*hxc_value).hxc_rootValue;
    hxc_result = hxc_tmp_class_field_load_result_n0;
  }
  else
  {
    if (hxc_other == NULL)
    {
      abort();
    }
    int32_t hxc_tmp_class_field_load_result_n1 = (*hxc_other).hxc_rootValue;
    hxc_result = hxc_tmp_class_field_load_result_n1;
  }
  int32_t hxc_tmp_load_result_n2 = hxc_result;
  if (hxc_value == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_class_field_load_result_n3 = (*hxc_value).hxc_rootValue;
  if (hxc_other == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_class_field_load_result_n4 = (*hxc_other).hxc_rootValue;
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_i32_add_wrapping(hxc_i32_add_wrapping(hxc_tmp_load_result_n2, hxc_tmp_class_field_load_result_n3), hxc_tmp_class_field_load_result_n4);
}

bool hxc_ClassLayoutFixture_different(struct hxc_RootRecord *hxc_left, struct hxc_RootRecord *hxc_right)
{
  const void *hxc_gc_roots[2] = { (const void *)hxc_left, (const void *)hxc_right };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 2, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_left != hxc_right;
}

bool hxc_ClassLayoutFixture_isNull(struct hxc_RootRecord *hxc_value)
{
  const void *hxc_gc_roots[2] = { (const void *)hxc_value, NULL };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 2, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_gc_roots[1] = (const void *)NULL;
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_value == NULL;
}

void hxc_ClassLayoutFixture_main(void)
{
  const void *hxc_gc_roots[19] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 19, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  hxc_gc_roots[0] = (const void *)NULL;
  struct hxc_LeafRecord *hxc_leaf = NULL;
  hxc_gc_roots[1] = (const void *)hxc_leaf;
  struct hxc_RootRecord *hxc_tmp_call_result_n1 = hxc_ClassLayoutFixture_asRoot(hxc_leaf);
  hxc_gc_roots[2] = (const void *)hxc_tmp_call_result_n1;
  struct hxc_RootRecord *hxc_root = hxc_tmp_call_result_n1;
  hxc_gc_roots[3] = (const void *)hxc_leaf;
  struct hxc_MiddleRecord *hxc_tmp_call_result_n3 = hxc_ClassLayoutFixture_asMiddle(hxc_leaf);
  hxc_gc_roots[4] = (const void *)hxc_tmp_call_result_n3;
  struct hxc_MiddleRecord *hxc_middle = hxc_tmp_call_result_n3;
  hxc_gc_roots[5] = (const void *)NULL;
  struct hxc_EmptyLeaf *hxc_emptyLeaf = NULL;
  hxc_gc_roots[6] = (const void *)hxc_emptyLeaf;
  struct hxc_EmptyBase *hxc_tmp_call_result_n5 = hxc_ClassLayoutFixture_asEmptyBase(hxc_emptyLeaf);
  hxc_gc_roots[7] = (const void *)hxc_tmp_call_result_n5;
  struct hxc_EmptyBase *hxc_emptyBase = hxc_tmp_call_result_n5;
  while (1)
  {
    struct hxc_RootRecord *hxc_tmp_load_result_n6 = hxc_root;
    hxc_gc_roots[8] = (const void *)hxc_tmp_load_result_n6;
    hxc_gc_roots[9] = (const void *)NULL;
    bool hxc_tmp_short_circuit_result_n5 = hxc_tmp_load_result_n6 == NULL;
    if (hxc_tmp_load_result_n6 == NULL)
    {
      hxc_gc_roots[10] = (const void *)hxc_middle;
      hxc_gc_roots[11] = (const void *)NULL;
      hxc_tmp_short_circuit_result_n5 = hxc_middle == NULL;
    }
    bool hxc_tmp_short_circuit_load_result_n8 = hxc_tmp_short_circuit_result_n5;
    bool hxc_tmp_short_circuit_result_n6 = hxc_tmp_short_circuit_load_result_n8;
    if (hxc_tmp_short_circuit_load_result_n8)
    {
      hxc_gc_roots[12] = (const void *)hxc_emptyBase;
      hxc_gc_roots[13] = (const void *)NULL;
      hxc_tmp_short_circuit_result_n6 = hxc_emptyBase == NULL;
    }
    bool hxc_tmp_short_circuit_load_result_n10 = hxc_tmp_short_circuit_result_n6;
    bool hxc_tmp_short_circuit_result_n7 = hxc_tmp_short_circuit_load_result_n10;
    if (hxc_tmp_short_circuit_load_result_n10)
    {
      hxc_gc_roots[14] = (const void *)hxc_root;
      hxc_gc_roots[15] = (const void *)NULL;
      bool hxc_tmp_call_result_n12 = hxc_ClassLayoutFixture_same(hxc_root, NULL);
      hxc_tmp_short_circuit_result_n7 = hxc_tmp_call_result_n12;
    }
    bool hxc_tmp_short_circuit_load_result_n13 = hxc_tmp_short_circuit_result_n7;
    bool hxc_tmp_short_circuit_result_n8 = hxc_tmp_short_circuit_load_result_n13;
    if (hxc_tmp_short_circuit_load_result_n13)
    {
      hxc_gc_roots[16] = (const void *)hxc_root;
      hxc_gc_roots[17] = (const void *)NULL;
      bool hxc_tmp_call_result_n15 = hxc_ClassLayoutFixture_different(hxc_root, NULL);
      hxc_tmp_short_circuit_result_n8 = !hxc_tmp_call_result_n15;
    }
    bool hxc_tmp_short_circuit_load_result_n16 = hxc_tmp_short_circuit_result_n8;
    bool hxc_tmp_short_circuit_result_n9 = hxc_tmp_short_circuit_load_result_n16;
    if (hxc_tmp_short_circuit_load_result_n16)
    {
      hxc_gc_roots[18] = (const void *)hxc_root;
      bool hxc_tmp_call_result_n18 = hxc_ClassLayoutFixture_isNull(hxc_root);
      hxc_tmp_short_circuit_result_n9 = hxc_tmp_call_result_n18;
    }
    if (!!hxc_tmp_short_circuit_result_n9)
    {
      break;
    }
  }
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return;
}

int32_t hxc_ClassLayoutFixture_readInherited(struct hxc_LeafRecord *hxc_value)
{
  const void *hxc_gc_roots[1] = { (const void *)hxc_value };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 1, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_value == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_class_field_load_result_n0 = (*hxc_value).hxc_base.hxc_base.hxc_rootValue;
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_tmp_class_field_load_result_n0;
}

struct hxc_RootRecord *hxc_ClassLayoutFixture_readPeer(struct hxc_LeafRecord *hxc_value)
{
  const void *hxc_gc_roots[2] = { (const void *)hxc_value, NULL };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 2, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_value == NULL)
  {
    abort();
  }
  struct hxc_RootRecord *hxc_tmp_class_field_load_result_n0 = (*hxc_value).hxc_peer;
  hxc_gc_roots[1] = (const void *)hxc_tmp_class_field_load_result_n0;
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_tmp_class_field_load_result_n0;
}

int32_t hxc_ClassLayoutFixture_readRoot(struct hxc_RootRecord *hxc_value)
{
  const void *hxc_gc_roots[1] = { (const void *)hxc_value };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 1, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_value == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_class_field_load_result_n0 = (*hxc_value).hxc_rootValue;
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_tmp_class_field_load_result_n0;
}

double hxc_ClassLayoutFixture_readScore(struct hxc_LeafRecord *hxc_value)
{
  const void *hxc_gc_roots[1] = { (const void *)hxc_value };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 1, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_value == NULL)
  {
    abort();
  }
  double hxc_tmp_class_field_load_result_n0 = (*hxc_value).hxc_score;
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_tmp_class_field_load_result_n0;
}

bool hxc_ClassLayoutFixture_same(struct hxc_RootRecord *hxc_left, struct hxc_RootRecord *hxc_right)
{
  const void *hxc_gc_roots[2] = { (const void *)hxc_left, (const void *)hxc_right };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 2, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_left == hxc_right;
}

double hxc_ClassLayoutFixture_sumAcrossBranch(struct hxc_LeafRecord *hxc_value, bool hxc_addScore)
{
  const void *hxc_gc_roots[1] = { (const void *)hxc_value };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 1, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_value == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_class_field_load_result_n0 = (*hxc_value).hxc_base.hxc_base.hxc_rootValue;
  double hxc_total = (double)hxc_tmp_class_field_load_result_n0;
  if (hxc_addScore)
  {
    double hxc_tmp_compound_load_result_n1 = hxc_total;
    double hxc_tmp_class_field_load_result_n2 = (*hxc_value).hxc_score;
    hxc_total = hxc_tmp_compound_load_result_n1 + hxc_tmp_class_field_load_result_n2;
  }
  double hxc_tmp_load_result_n3 = hxc_total;
  int32_t hxc_tmp_class_field_load_result_n4 = (*hxc_value).hxc_base.hxc_base.hxc_rootValue;
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_tmp_load_result_n3 + (double)hxc_tmp_class_field_load_result_n4;
}

int32_t hxc_ClassLayoutFixture_writeInherited(struct hxc_LeafRecord *hxc_value, int32_t hxc_next)
{
  const void *hxc_gc_roots[1] = { (const void *)hxc_value };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 1, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_value == NULL)
  {
    abort();
  }
  (*hxc_value).hxc_base.hxc_base.hxc_rootValue = hxc_next;
  int32_t hxc_tmp_class_field_load_result_n0 = (*hxc_value).hxc_base.hxc_base.hxc_rootValue;
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_tmp_class_field_load_result_n0;
}

struct hxc_RootRecord *hxc_ClassLayoutFixture_writePeer(struct hxc_LeafRecord *hxc_value, struct hxc_RootRecord *hxc_peer)
{
  const void *hxc_gc_roots[3] = { (const void *)hxc_value, (const void *)hxc_peer, NULL };
  struct hxc_gc_root_frame hxc_gc_frame = HXC_GC_ROOT_FRAME_INITIALIZER;
  if (hxc_gc_root_frame_push(&hxc_program_gc_thread, hxc_gc_roots, 3, &hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  if (hxc_value == NULL)
  {
    abort();
  }
  (*hxc_value).hxc_peer = hxc_peer;
  struct hxc_RootRecord *hxc_tmp_class_field_load_result_n0 = (*hxc_value).hxc_peer;
  hxc_gc_roots[2] = (const void *)hxc_tmp_class_field_load_result_n0;
  if (hxc_gc_root_frame_pop(&hxc_gc_frame) != HXC_STATUS_OK)
  {
    abort();
  }
  return hxc_tmp_class_field_load_result_n0;
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
  hxc_ClassLayoutFixture_main();
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
