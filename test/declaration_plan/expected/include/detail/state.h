#ifndef HXC_DETAIL_STATE_H_INCLUDED
#define HXC_DETAIL_STATE_H_INCLUDED

#include "project/config.h"
#include "types/value.h"

struct hidden_handle;

typedef struct hidden_handle hidden_handle;

struct private_state;

typedef struct private_state private_state;

struct private_state {
  value current;
  hidden_handle *hidden;
  project_config_tag config_tag;
};

#endif /* HXC_DETAIL_STATE_H_INCLUDED */
