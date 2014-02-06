
/**
 * `var.c` - sly
 *
 * copyright (c) 2014 - joseph werle <joseph.werle@gmail.com>
 */

#include <stdlib.h>
#include <sly/value.h>
#include <sly/var.h>

sly_var_t *
sly_var_new (sly_value_t *value) {
  sly_var_t *self = (sly_var_t *) malloc(sizeof(sly_var_t));
  if (NULL == self) return NULL;
  self->value.as_pointer = value;
  self->class = "Var";
  self->type = SLY_NODE_VAR;
  return self;
}

sly_value_conv_t *
sly_var_value (sly_var_t *self) {
  sly_value_t *value = (sly_value_t *) self->value.as_pointer;
  return (sly_value_conv_t *) &value->value;
}
