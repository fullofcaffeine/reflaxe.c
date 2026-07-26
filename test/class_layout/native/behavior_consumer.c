/*
 * This file is an independent C consumer, not input to haxe.c. It includes the
 * generated private program so the test can check the exact C layout and call
 * convention from the viewpoint of ordinary C code. Writing this boundary in
 * Haxe would compile both sides with haxe.c and could let the same compiler bug
 * make producer and consumer agree incorrectly.
 *
 * The retained LeafRecord.peer reference makes part of this fixture a
 * collector-managed class graph. Generated managed methods publish exact root
 * frames, so this consumer starts the generated program's collector before it
 * calls those methods. The objects below still use caller-owned automatic
 * storage; collector startup tests the generated call protocol, not allocation.
 */
#define main hxc_class_layout_generated_main
int main(void);
#include "../src/program.c"
#undef main

#include <stdint.h>

int main(void);

int main(void)
{
  if (hxc_gc_init(&(struct hxc_gc_config){
        hxc_default_allocator(),
        1048576U,
        NULL,
        NULL,
      }, &hxc_program_gc) != HXC_STATUS_OK ||
      hxc_gc_thread_register(&hxc_program_gc, &hxc_program_gc_thread) != HXC_STATUS_OK) {
    return 10;
  }
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
  if (HXC_FN_SUM_ACROSS_BRANCH(&leaf, true) != 24.5 ||
      HXC_FN_SUM_ACROSS_BRANCH(&leaf, false) != 22.0 ||
      HXC_FN_BRANCH_PROOF_DOES_NOT_ESCAPE(&leaf.HXC_LEAF_BASE.HXC_MIDDLE_BASE, &root, true) != INT32_C(29) ||
      HXC_FN_BRANCH_PROOF_DOES_NOT_ESCAPE(&leaf.HXC_LEAF_BASE.HXC_MIDDLE_BASE, &root, false) != INT32_C(25)) {
    return 5;
  }
  if (hxc_gc_thread_unregister(&hxc_program_gc_thread) != HXC_STATUS_OK ||
      hxc_gc_dispose(&hxc_program_gc) != HXC_STATUS_OK) {
    return 11;
  }
  return 0;
}
