
/**
 * `let.c` - sly
 *
 * copyright (c) 2014 - joseph werle <joseph.werle@gmail.com>
 */

#include <stdlib.h>
#include <sly/let.h>

SLY_EXTERN sly_let_t *
sly_let_new () {
  sly_let_t *self = (sly_let_t *) malloc(sizeof(sly_let_t));
  if (NULL == self) return NULL;
  self->class = "Let";
  return self;
}
