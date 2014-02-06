
/**
 * `parser.c` - sly
 *
 * copyright (c) 2014 - joseph werle <joseph.werle@gmail.com>
 */

#include <stdlib.h>
#include <sly/parser.h>

SLY_EXTERN sly_parser_t *
sly_parser_new () {
  sly_parser_t *self = (sly_parser_t *) malloc(sizeof(sly_parser_t));
  if (NULL == self) return NULL;
  return self;
}
