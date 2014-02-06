
/**
 * `parser.h' - (c) 2014 joseph werle - joseph.werle@gmail.com
 */

#ifndef SLY_PARSER_H
#define SLY_PARSER_H 1

#include "common.h"

typedef struct sly_parser_s {
  const char *src;
  char *ctx;
  char *error;
} sly_parser_t;

SLY_EXTERN sly_parser_t *
sly_parser_new (const char *);

#endif
