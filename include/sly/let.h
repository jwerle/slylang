
/**
 * `let.h` - sly
 *
 * copyright (c) 2014 - joseph werle <joseph.werle@gmail.com>
 */

#ifndef SLY_LET_H
#define SLY_LET_H 1

#include "common.h"
#include "value.h"

typedef struct sly_let_s {
  SLY_VALUE_FIELDS(void *)
} sly_let_t;

SLY_EXTERN sly_let_t *
sly_let_new ();

#endif
