#ifndef HXC_PROGRAM_H_INCLUDED
#define HXC_PROGRAM_H_INCLUDED

#include <hxrt/gc.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef char hxc_runtime_abi_major_must_match[HXC_RUNTIME_ABI_MAJOR == 0U ? 1 : -1];

extern struct hxc_gc hxc_program_gc;

extern struct hxc_gc_thread hxc_program_gc_thread;

extern const struct hxc_type_descriptor hxc_EmptyBase_descriptor;

extern const struct hxc_type_descriptor hxc_EmptyLeaf_descriptor;

extern const struct hxc_type_descriptor hxc_MiddleRecord_descriptor;

extern const struct hxc_type_descriptor hxc_LeafRecord_descriptor;

extern const struct hxc_type_descriptor hxc_RootRecord_descriptor;

static inline int32_t hxc_u32_to_i32_bits(uint32_t hxc_l_value)
{
  if (hxc_l_value <= UINT32_C(2147483647))
  {
    return (int32_t)hxc_l_value;
  }
  return INT32_MIN + (int32_t)(hxc_l_value - UINT32_C(2147483648));
}

static inline int32_t hxc_i32_add_wrapping(int32_t hxc_l_left, int32_t hxc_l_right)
{
  return hxc_u32_to_i32_bits((uint32_t)((uint64_t)(uint32_t)hxc_l_left + (uint64_t)(uint32_t)hxc_l_right));
}

struct hxc_RootRecord;

struct hxc_MiddleRecord;

struct hxc_LeafRecord;

struct hxc_EmptyBase;

struct hxc_EmptyLeaf;

struct hxc_EmptyBase {
  unsigned char hxc_storage;
};

struct hxc_EmptyLeaf {
  struct hxc_EmptyBase hxc_base;
};

struct hxc_RootRecord {
  int32_t hxc_rootValue;
};

struct hxc_MiddleRecord {
  struct hxc_RootRecord hxc_base;
  bool hxc_enabled;
};

struct hxc_LeafRecord {
  struct hxc_MiddleRecord hxc_base;
  double hxc_score;
  struct hxc_RootRecord *hxc_peer;
};

struct hxc_EmptyBase *hxc_ClassLayoutFixture_asEmptyBase(struct hxc_EmptyLeaf *hxc_l_value);

struct hxc_MiddleRecord *hxc_ClassLayoutFixture_asMiddle(struct hxc_LeafRecord *hxc_l_value);

struct hxc_RootRecord *hxc_ClassLayoutFixture_asRoot(struct hxc_LeafRecord *hxc_l_value);

int32_t hxc_ClassLayoutFixture_branchProofDoesNotEscape(struct hxc_RootRecord *hxc_l_value, struct hxc_RootRecord *hxc_l_other, bool hxc_l_useFirst);

bool hxc_ClassLayoutFixture_different(struct hxc_RootRecord *hxc_l_left, struct hxc_RootRecord *hxc_l_right);

bool hxc_ClassLayoutFixture_isNull(struct hxc_RootRecord *hxc_l_value);

void hxc_ClassLayoutFixture_main(void);

int32_t hxc_ClassLayoutFixture_readInherited(struct hxc_LeafRecord *hxc_l_value);

struct hxc_RootRecord *hxc_ClassLayoutFixture_readPeer(struct hxc_LeafRecord *hxc_l_value);

int32_t hxc_ClassLayoutFixture_readRoot(struct hxc_RootRecord *hxc_l_value);

double hxc_ClassLayoutFixture_readScore(struct hxc_LeafRecord *hxc_l_value);

bool hxc_ClassLayoutFixture_same(struct hxc_RootRecord *hxc_l_left, struct hxc_RootRecord *hxc_l_right);

double hxc_ClassLayoutFixture_sumAcrossBranch(struct hxc_LeafRecord *hxc_l_value, bool hxc_l_addScore);

int32_t hxc_ClassLayoutFixture_writeInherited(struct hxc_LeafRecord *hxc_l_value, int32_t hxc_l_next);

struct hxc_RootRecord *hxc_ClassLayoutFixture_writePeer(struct hxc_LeafRecord *hxc_l_value, struct hxc_RootRecord *hxc_l_peer);

#endif /* HXC_PROGRAM_H_INCLUDED */
