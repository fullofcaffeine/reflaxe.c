#ifndef HXC_NODES_H_INCLUDED
#define HXC_NODES_H_INCLUDED

#include <stdint.h>

struct node_a;

typedef struct node_a node_a;

struct node_b;

typedef struct node_b node_b;

struct node_a {
  node_b *next;
  int32_t value;
};

struct node_b {
  node_a *previous;
  int32_t value;
};

#endif /* HXC_NODES_H_INCLUDED */
