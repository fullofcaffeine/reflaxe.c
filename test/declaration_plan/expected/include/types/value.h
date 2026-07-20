#ifndef HXC_TYPES_VALUE_H_INCLUDED
#define HXC_TYPES_VALUE_H_INCLUDED

#include <stdint.h>

struct value;

typedef struct value value;

struct value {
  int32_t scalar;
};

#endif /* HXC_TYPES_VALUE_H_INCLUDED */
