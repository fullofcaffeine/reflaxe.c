#include "hxc/program.h"

#include <cstddef>
#include <cstdint>

extern "C" {
std::size_t hxc_test_root_size(void);
std::size_t hxc_test_root_alignment(void);
std::size_t hxc_test_root_value_offset(void);
std::size_t hxc_test_middle_size(void);
std::size_t hxc_test_middle_alignment(void);
std::size_t hxc_test_middle_base_offset(void);
std::size_t hxc_test_middle_enabled_offset(void);
std::size_t hxc_test_leaf_size(void);
std::size_t hxc_test_leaf_alignment(void);
std::size_t hxc_test_leaf_base_offset(void);
std::size_t hxc_test_leaf_score_offset(void);
std::size_t hxc_test_leaf_peer_offset(void);
std::size_t hxc_test_empty_base_size(void);
std::size_t hxc_test_empty_leaf_size(void);
std::size_t hxc_test_empty_leaf_base_offset(void);
}

int main()
{
  HXC_ROOT_TAG root{};
  root.HXC_ROOT_VALUE = INT32_C(7);
  HXC_MIDDLE_TAG middle{};
  middle.HXC_MIDDLE_BASE = root;
  middle.HXC_MIDDLE_ENABLED = true;
  HXC_LEAF_TAG leaf{};
  leaf.HXC_LEAF_BASE = middle;
  leaf.HXC_LEAF_SCORE = 2.5;
  leaf.HXC_LEAF_PEER = &root;

  const bool layout_matches =
    hxc_test_root_size() == sizeof(HXC_ROOT_TAG) &&
    hxc_test_root_alignment() == alignof(HXC_ROOT_TAG) &&
    hxc_test_root_value_offset() == offsetof(HXC_ROOT_TAG, HXC_ROOT_VALUE) &&
    hxc_test_middle_size() == sizeof(HXC_MIDDLE_TAG) &&
    hxc_test_middle_alignment() == alignof(HXC_MIDDLE_TAG) &&
    hxc_test_middle_base_offset() == offsetof(HXC_MIDDLE_TAG, HXC_MIDDLE_BASE) &&
    hxc_test_middle_enabled_offset() == offsetof(HXC_MIDDLE_TAG, HXC_MIDDLE_ENABLED) &&
    hxc_test_leaf_size() == sizeof(HXC_LEAF_TAG) &&
    hxc_test_leaf_alignment() == alignof(HXC_LEAF_TAG) &&
    hxc_test_leaf_base_offset() == offsetof(HXC_LEAF_TAG, HXC_LEAF_BASE) &&
    hxc_test_leaf_score_offset() == offsetof(HXC_LEAF_TAG, HXC_LEAF_SCORE) &&
    hxc_test_leaf_peer_offset() == offsetof(HXC_LEAF_TAG, HXC_LEAF_PEER) &&
    hxc_test_empty_base_size() == sizeof(HXC_EMPTY_BASE_TAG) &&
    hxc_test_empty_leaf_size() == sizeof(HXC_EMPTY_LEAF_TAG) &&
    hxc_test_empty_leaf_base_offset() == offsetof(HXC_EMPTY_LEAF_TAG, HXC_EMPTY_LEAF_BASE);
  const bool values_match =
    leaf.HXC_LEAF_BASE.HXC_MIDDLE_BASE.HXC_ROOT_VALUE == INT32_C(7) &&
    leaf.HXC_LEAF_BASE.HXC_MIDDLE_ENABLED &&
    leaf.HXC_LEAF_SCORE == 2.5 &&
    leaf.HXC_LEAF_PEER == &root;
  return layout_matches && values_match ? 0 : 1;
}
