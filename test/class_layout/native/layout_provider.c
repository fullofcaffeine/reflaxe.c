#include "hxc/program.h"

#include <stddef.h>

size_t hxc_test_root_size(void);
size_t hxc_test_root_alignment(void);
size_t hxc_test_root_value_offset(void);
size_t hxc_test_middle_size(void);
size_t hxc_test_middle_alignment(void);
size_t hxc_test_middle_base_offset(void);
size_t hxc_test_middle_enabled_offset(void);
size_t hxc_test_leaf_size(void);
size_t hxc_test_leaf_alignment(void);
size_t hxc_test_leaf_base_offset(void);
size_t hxc_test_leaf_score_offset(void);
size_t hxc_test_leaf_peer_offset(void);
size_t hxc_test_empty_base_size(void);
size_t hxc_test_empty_leaf_size(void);
size_t hxc_test_empty_leaf_base_offset(void);

size_t hxc_test_root_size(void) { return sizeof(struct HXC_ROOT_TAG); }
size_t hxc_test_root_alignment(void) { return _Alignof(struct HXC_ROOT_TAG); }
size_t hxc_test_root_value_offset(void) { return offsetof(struct HXC_ROOT_TAG, HXC_ROOT_VALUE); }
size_t hxc_test_middle_size(void) { return sizeof(struct HXC_MIDDLE_TAG); }
size_t hxc_test_middle_alignment(void) { return _Alignof(struct HXC_MIDDLE_TAG); }
size_t hxc_test_middle_base_offset(void) { return offsetof(struct HXC_MIDDLE_TAG, HXC_MIDDLE_BASE); }
size_t hxc_test_middle_enabled_offset(void) { return offsetof(struct HXC_MIDDLE_TAG, HXC_MIDDLE_ENABLED); }
size_t hxc_test_leaf_size(void) { return sizeof(struct HXC_LEAF_TAG); }
size_t hxc_test_leaf_alignment(void) { return _Alignof(struct HXC_LEAF_TAG); }
size_t hxc_test_leaf_base_offset(void) { return offsetof(struct HXC_LEAF_TAG, HXC_LEAF_BASE); }
size_t hxc_test_leaf_score_offset(void) { return offsetof(struct HXC_LEAF_TAG, HXC_LEAF_SCORE); }
size_t hxc_test_leaf_peer_offset(void) { return offsetof(struct HXC_LEAF_TAG, HXC_LEAF_PEER); }
size_t hxc_test_empty_base_size(void) { return sizeof(struct HXC_EMPTY_BASE_TAG); }
size_t hxc_test_empty_leaf_size(void) { return sizeof(struct HXC_EMPTY_LEAF_TAG); }
size_t hxc_test_empty_leaf_base_offset(void) { return offsetof(struct HXC_EMPTY_LEAF_TAG, HXC_EMPTY_LEAF_BASE); }
