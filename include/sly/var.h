
/**
 * `var.h` - sly
 *
 * copyright (c) 2014 - joseph werle <joseph.werle@gmail.com>
 */

#ifndef SLY_VAR_H
#define SLY_VAR_H 1

#include "common.h"
#include "value.h"

typedef struct sly_var_s {
  SLY_VALUE_FIELDS(sly_value_t *)
} sly_var_t;

SLY_EXTERN sly_var_t *
sly_var_new (sly_value_t *);

SLY_EXTERN sly_value_conv_t *
sly_var_value (sly_var_t *);

#endif
