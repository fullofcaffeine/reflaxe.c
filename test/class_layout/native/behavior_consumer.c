#define main hxc_class_layout_generated_main
int main(void);
#include "../src/program.c"
#undef main

#include <stdint.h>

int main(void);

int main(void)
{
  struct HXC_ROOT_TAG root = {
    .HXC_ROOT_VALUE = INT32_C(7),
  };
  struct HXC_MIDDLE_TAG middle = {
    .HXC_MIDDLE_BASE = root,
    .HXC_MIDDLE_ENABLED = true,
  };
  struct HXC_LEAF_TAG leaf = {
    .HXC_LEAF_BASE = middle,
    .HXC_LEAF_SCORE = 2.5,
    .HXC_LEAF_PEER = &root,
  };
  struct HXC_EMPTY_LEAF_TAG empty_leaf = {
    .HXC_EMPTY_LEAF_BASE = {
      .HXC_EMPTY_BASE_ANCHOR = 0,
    },
  };

  struct HXC_ROOT_TAG *root_view = HXC_FN_AS_ROOT(&leaf);
  struct HXC_MIDDLE_TAG *middle_view = HXC_FN_AS_MIDDLE(&leaf);
  struct HXC_EMPTY_BASE_TAG *empty_view = HXC_FN_AS_EMPTY_BASE(&empty_leaf);
  if (root_view != &leaf.HXC_LEAF_BASE.HXC_MIDDLE_BASE ||
      middle_view != &leaf.HXC_LEAF_BASE ||
      empty_view != &empty_leaf.HXC_EMPTY_LEAF_BASE) {
    return 1;
  }
  if (HXC_FN_READ_ROOT(root_view) != INT32_C(7) ||
      HXC_FN_READ_INHERITED(&leaf) != INT32_C(7) ||
      HXC_FN_READ_SCORE(&leaf) != 2.5 ||
      HXC_FN_READ_PEER(&leaf) != &root) {
    return 2;
  }
  if (HXC_FN_WRITE_INHERITED(&leaf, INT32_C(11)) != INT32_C(11) ||
      root_view->HXC_ROOT_VALUE != INT32_C(11)) {
    return 3;
  }
  if (HXC_FN_WRITE_PEER(&leaf, root_view) != root_view ||
      !HXC_FN_SAME(root_view, root_view) ||
      HXC_FN_DIFFERENT(root_view, root_view) ||
      HXC_FN_IS_NULL(root_view) ||
      !HXC_FN_IS_NULL(NULL) ||
      HXC_FN_AS_ROOT(NULL) != NULL) {
    return 4;
  }
  return 0;
}
