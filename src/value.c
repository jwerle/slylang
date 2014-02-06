
/**
 * `value.c` - sly
 *
 * copyright (c) 2014 - joseph werle <joseph.werle@gmail.com>
 */

#include <stdlib.h>
#include <sly/value.h>

SLY_EXTERN sly_value_t *
sly_value_new (void *value) {
  sly_value_t *self = (sly_value_t *) malloc(sizeof(sly_value_t));
  if (NULL == self) return NULL;
  self->value.as_self = value;
  self->class = "Value";
  self->type = SLY_NODE_GENERIC;
  if (NULL == value) {
    self->type = SLY_NODE_UNDEFINED;
  }
  return self;
}

void
sly_value_free (void *_) {
  sly_value_t *self = (sly_value_t *) _;
  free(self);
}
