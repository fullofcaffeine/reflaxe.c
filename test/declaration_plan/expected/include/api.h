#ifndef HXC_GENERATED_PATH_6170692E68_INCLUDED
#define HXC_GENERATED_PATH_6170692E68_INCLUDED

#include <clock_api.h>
#include <stddef.h>

#include "project/config.h"
#include "types/value.h"

struct api_envelope;

typedef struct api_envelope api_envelope;

struct api_request;

typedef struct api_request api_request;

struct hidden_handle;

typedef struct hidden_handle hidden_handle;

struct node_a;

typedef struct node_a node_a;

struct private_state;

typedef struct private_state private_state;

struct api_request {
  value payload;
  node_a *node;
  hidden_handle *hidden;
  external_clock *clock;
  private_state *state;
  size_t count;
  project_config_tag config_tag;
};

struct api_envelope {
  api_request request;
};

#endif /* HXC_GENERATED_PATH_6170692E68_INCLUDED */
