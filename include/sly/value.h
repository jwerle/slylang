
/**
 * `value.h` - sly
 *
 * copyright (c) 2014 - joseph werle <joseph.werle@gmail.com>
 */

#ifndef SLY_VALUE_H
#define SLY_VALUE_H 1

#include "common.h"
#include "node.h"

#define SLY_VALUE_UNION_FIELDS \
  void *as_pointer;            \
  int as_int;                  \
  char as_char;                \

#define SLY_VALUE_FIELDS(t)    \
  const char *class;           \
  sly_node_type_t type;        \
  union {                      \
    t as_self;                 \
    SLY_VALUE_UNION_FIELDS     \
  } value;                     \

typedef union sly_value_conv {
  SLY_VALUE_UNION_FIELDS
} sly_value_conv_t;

typedef struct sly_value_s {
  SLY_VALUE_FIELDS(void *)
} sly_value_t;

SLY_EXTERN sly_value_t *
sly_value_new (void *value);

SLY_EXTERN void
sly_value_free (void *);

#endif
