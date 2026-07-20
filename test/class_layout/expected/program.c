#include "hxc/program.h"

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

struct hxc_EmptyBase *hxc_ClassLayoutFixture_asEmptyBase(struct hxc_EmptyLeaf *hxc_value)
{
  return hxc_value == NULL ? NULL : &hxc_value->hxc_base;
}

struct hxc_MiddleRecord *hxc_ClassLayoutFixture_asMiddle(struct hxc_LeafRecord *hxc_value)
{
  return hxc_value == NULL ? NULL : &hxc_value->hxc_base;
}

struct hxc_RootRecord *hxc_ClassLayoutFixture_asRoot(struct hxc_LeafRecord *hxc_value)
{
  return hxc_value == NULL ? NULL : &hxc_value->hxc_base.hxc_base;
}

bool hxc_ClassLayoutFixture_different(struct hxc_RootRecord *hxc_left, struct hxc_RootRecord *hxc_right)
{
  return hxc_left != hxc_right;
}

bool hxc_ClassLayoutFixture_isNull(struct hxc_RootRecord *hxc_value)
{
  return hxc_value == NULL;
}

void hxc_ClassLayoutFixture_main(void)
{
  struct hxc_LeafRecord *hxc_leaf = NULL;
  struct hxc_RootRecord *hxc_tmp_call_result_n1 = hxc_ClassLayoutFixture_asRoot(hxc_leaf);
  struct hxc_RootRecord *hxc_root = hxc_tmp_call_result_n1;
  struct hxc_MiddleRecord *hxc_tmp_call_result_n3 = hxc_ClassLayoutFixture_asMiddle(hxc_leaf);
  struct hxc_MiddleRecord *hxc_middle = hxc_tmp_call_result_n3;
  struct hxc_EmptyLeaf *hxc_emptyLeaf = NULL;
  struct hxc_EmptyBase *hxc_tmp_call_result_n5 = hxc_ClassLayoutFixture_asEmptyBase(hxc_emptyLeaf);
  struct hxc_EmptyBase *hxc_emptyBase = hxc_tmp_call_result_n5;
  while (1)
  {
    struct hxc_RootRecord *hxc_tmp_load_result_n6 = hxc_root;
    bool hxc_tmp_short_circuit_result_n5 = hxc_tmp_load_result_n6 == NULL;
    if (hxc_tmp_load_result_n6 == NULL)
    {
      hxc_tmp_short_circuit_result_n5 = hxc_middle == NULL;
    }
    bool hxc_tmp_short_circuit_load_result_n8 = hxc_tmp_short_circuit_result_n5;
    bool hxc_tmp_short_circuit_result_n6 = hxc_tmp_short_circuit_load_result_n8;
    if (hxc_tmp_short_circuit_load_result_n8)
    {
      hxc_tmp_short_circuit_result_n6 = hxc_emptyBase == NULL;
    }
    bool hxc_tmp_short_circuit_load_result_n10 = hxc_tmp_short_circuit_result_n6;
    bool hxc_tmp_short_circuit_result_n7 = hxc_tmp_short_circuit_load_result_n10;
    if (hxc_tmp_short_circuit_load_result_n10)
    {
      bool hxc_tmp_call_result_n12 = hxc_ClassLayoutFixture_same(hxc_root, NULL);
      hxc_tmp_short_circuit_result_n7 = hxc_tmp_call_result_n12;
    }
    bool hxc_tmp_short_circuit_load_result_n13 = hxc_tmp_short_circuit_result_n7;
    bool hxc_tmp_short_circuit_result_n8 = hxc_tmp_short_circuit_load_result_n13;
    if (hxc_tmp_short_circuit_load_result_n13)
    {
      bool hxc_tmp_call_result_n15 = hxc_ClassLayoutFixture_different(hxc_root, NULL);
      hxc_tmp_short_circuit_result_n8 = !hxc_tmp_call_result_n15;
    }
    bool hxc_tmp_short_circuit_load_result_n16 = hxc_tmp_short_circuit_result_n8;
    bool hxc_tmp_short_circuit_result_n9 = hxc_tmp_short_circuit_load_result_n16;
    if (hxc_tmp_short_circuit_load_result_n16)
    {
      bool hxc_tmp_call_result_n18 = hxc_ClassLayoutFixture_isNull(hxc_root);
      hxc_tmp_short_circuit_result_n9 = hxc_tmp_call_result_n18;
    }
    if (!!hxc_tmp_short_circuit_result_n9)
    {
      break;
    }
  }
  return;
}

int32_t hxc_ClassLayoutFixture_readInherited(struct hxc_LeafRecord *hxc_value)
{
  if (hxc_value == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_class_field_load_result_n0 = (*hxc_value).hxc_base.hxc_base.hxc_rootValue;
  return hxc_tmp_class_field_load_result_n0;
}

struct hxc_RootRecord *hxc_ClassLayoutFixture_readPeer(struct hxc_LeafRecord *hxc_value)
{
  if (hxc_value == NULL)
  {
    abort();
  }
  struct hxc_RootRecord *hxc_tmp_class_field_load_result_n0 = (*hxc_value).hxc_peer;
  return hxc_tmp_class_field_load_result_n0;
}

int32_t hxc_ClassLayoutFixture_readRoot(struct hxc_RootRecord *hxc_value)
{
  if (hxc_value == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_class_field_load_result_n0 = (*hxc_value).hxc_rootValue;
  return hxc_tmp_class_field_load_result_n0;
}

double hxc_ClassLayoutFixture_readScore(struct hxc_LeafRecord *hxc_value)
{
  if (hxc_value == NULL)
  {
    abort();
  }
  double hxc_tmp_class_field_load_result_n0 = (*hxc_value).hxc_score;
  return hxc_tmp_class_field_load_result_n0;
}

bool hxc_ClassLayoutFixture_same(struct hxc_RootRecord *hxc_left, struct hxc_RootRecord *hxc_right)
{
  return hxc_left == hxc_right;
}

int32_t hxc_ClassLayoutFixture_writeInherited(struct hxc_LeafRecord *hxc_value, int32_t hxc_next)
{
  if (hxc_value == NULL)
  {
    abort();
  }
  (*hxc_value).hxc_base.hxc_base.hxc_rootValue = hxc_next;
  if (hxc_value == NULL)
  {
    abort();
  }
  int32_t hxc_tmp_class_field_load_result_n0 = (*hxc_value).hxc_base.hxc_base.hxc_rootValue;
  return hxc_tmp_class_field_load_result_n0;
}

struct hxc_RootRecord *hxc_ClassLayoutFixture_writePeer(struct hxc_LeafRecord *hxc_value, struct hxc_RootRecord *hxc_peer)
{
  if (hxc_value == NULL)
  {
    abort();
  }
  (*hxc_value).hxc_peer = hxc_peer;
  if (hxc_value == NULL)
  {
    abort();
  }
  struct hxc_RootRecord *hxc_tmp_class_field_load_result_n0 = (*hxc_value).hxc_peer;
  return hxc_tmp_class_field_load_result_n0;
}

int main(void)
{
  hxc_ClassLayoutFixture_main();
  return 0;
}
