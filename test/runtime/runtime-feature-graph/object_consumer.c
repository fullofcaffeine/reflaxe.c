/* Independent native behavior evidence for the selectively packaged object slice. */
#include "hxrt/object.h"

#include <stdio.h>

struct fixture_node {
  struct fixture_node *next;
  int32_t value;
};

struct fixture_observation {
  const void *visited;
  size_t visits;
  size_t finalizations;
};

static struct fixture_observation fixture_state = { NULL, 0u, 0u };

static void fixture_visit(void *context, const void *managed_object) {
  struct fixture_observation *observation = context;
  observation->visited = managed_object;
  observation->visits += 1u;
}

static void fixture_trace(
  const void *object,
  hxc_trace_visit_fn visit,
  void *visit_context
) {
  const struct fixture_node *node = object;
  if (node->next != NULL) {
    visit(visit_context, node->next);
  }
}

static void fixture_finalize(void *object) {
  struct fixture_node *node = object;
  node->value = -1;
  fixture_state.finalizations += 1u;
}

static const hxc_type_descriptor fixture_leaf_descriptor = {
  HXC_TYPE_DESCRIPTOR_ABI_VERSION,
  UINT32_C(0),
  sizeof(int32_t),
  HXC_ALIGNOF(int32_t),
  NULL,
  NULL
};

static const hxc_type_descriptor fixture_node_descriptor = {
  HXC_TYPE_DESCRIPTOR_ABI_VERSION,
  HXC_TYPE_DESCRIPTOR_HAS_TRACE | HXC_TYPE_DESCRIPTOR_HAS_FINALIZER,
  sizeof(struct fixture_node),
  HXC_ALIGNOF(struct fixture_node),
  fixture_trace,
  fixture_finalize
};

static int fixture_expect(bool condition, int code) {
  return condition ? 0 : code;
}

int main(void) {
  struct fixture_node tail = { NULL, 7 };
  struct fixture_node head = { &tail, 9 };
  hxc_object_header header = { NULL };
  hxc_type_descriptor malformed = fixture_node_descriptor;
  int result;

  result = fixture_expect(hxc_type_descriptor_is_valid(&fixture_leaf_descriptor), 1);
  if (result != 0) {
    return result;
  }
  result = fixture_expect(hxc_type_descriptor_is_valid(&fixture_node_descriptor), 2);
  if (result != 0) {
    return result;
  }
  result = fixture_expect(hxc_object_header_init(&fixture_node_descriptor, &header), 3);
  if (result != 0) {
    return result;
  }
  result = fixture_expect(hxc_object_header_is_valid(&header), 4);
  if (result != 0) {
    return result;
  }
  result = fixture_expect(
    hxc_type_descriptor_trace(&fixture_node_descriptor, &head, fixture_visit, &fixture_state),
    5
  );
  if (result != 0) {
    return result;
  }
  result = fixture_expect(fixture_state.visits == 1u && fixture_state.visited == &tail, 6);
  if (result != 0) {
    return result;
  }
  result = fixture_expect(
    hxc_type_descriptor_trace(&fixture_leaf_descriptor, &head.value, NULL, NULL),
    7
  );
  if (result != 0) {
    return result;
  }
  result = fixture_expect(hxc_type_descriptor_finalize(&fixture_node_descriptor, &head), 8);
  if (result != 0) {
    return result;
  }
  result = fixture_expect(head.value == -1 && fixture_state.finalizations == 1u, 9);
  if (result != 0) {
    return result;
  }

  malformed.abi_version += UINT32_C(1);
  result = fixture_expect(!hxc_type_descriptor_is_valid(&malformed), 10);
  if (result != 0) {
    return result;
  }
  malformed = fixture_node_descriptor;
  malformed.flags = UINT32_C(4);
  result = fixture_expect(!hxc_type_descriptor_is_valid(&malformed), 11);
  if (result != 0) {
    return result;
  }
  malformed = fixture_node_descriptor;
  malformed.trace = NULL;
  result = fixture_expect(!hxc_type_descriptor_is_valid(&malformed), 12);
  if (result != 0) {
    return result;
  }
  malformed = fixture_node_descriptor;
  malformed.object_alignment = 3u;
  result = fixture_expect(!hxc_type_descriptor_is_valid(&malformed), 13);
  if (result != 0) {
    return result;
  }
  result = fixture_expect(
    !hxc_type_descriptor_trace(&fixture_node_descriptor, &head, NULL, NULL),
    14
  );
  if (result != 0) {
    return result;
  }

  puts("runtime-feature-object: OK");
  return 0;
}
