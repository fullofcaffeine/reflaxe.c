#ifndef HXC_PROGRAM_H_INCLUDED
#define HXC_PROGRAM_H_INCLUDED

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

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

struct hxc_EmptyBase *hxc_ClassLayoutFixture_asEmptyBase(struct hxc_EmptyLeaf *hxc_value);

struct hxc_MiddleRecord *hxc_ClassLayoutFixture_asMiddle(struct hxc_LeafRecord *hxc_value);

struct hxc_RootRecord *hxc_ClassLayoutFixture_asRoot(struct hxc_LeafRecord *hxc_value);

int32_t hxc_ClassLayoutFixture_branchProofDoesNotEscape(struct hxc_RootRecord *hxc_value, struct hxc_RootRecord *hxc_other, bool hxc_useFirst);

bool hxc_ClassLayoutFixture_different(struct hxc_RootRecord *hxc_left, struct hxc_RootRecord *hxc_right);

bool hxc_ClassLayoutFixture_isNull(struct hxc_RootRecord *hxc_value);

void hxc_ClassLayoutFixture_main(void);

int32_t hxc_ClassLayoutFixture_readInherited(struct hxc_LeafRecord *hxc_value);

struct hxc_RootRecord *hxc_ClassLayoutFixture_readPeer(struct hxc_LeafRecord *hxc_value);

int32_t hxc_ClassLayoutFixture_readRoot(struct hxc_RootRecord *hxc_value);

double hxc_ClassLayoutFixture_readScore(struct hxc_LeafRecord *hxc_value);

bool hxc_ClassLayoutFixture_same(struct hxc_RootRecord *hxc_left, struct hxc_RootRecord *hxc_right);

double hxc_ClassLayoutFixture_sumAcrossBranch(struct hxc_LeafRecord *hxc_value, bool hxc_addScore);

int32_t hxc_ClassLayoutFixture_writeInherited(struct hxc_LeafRecord *hxc_value, int32_t hxc_next);

struct hxc_RootRecord *hxc_ClassLayoutFixture_writePeer(struct hxc_LeafRecord *hxc_value, struct hxc_RootRecord *hxc_peer);

#endif /* HXC_PROGRAM_H_INCLUDED */
