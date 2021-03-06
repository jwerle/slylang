
/**
 * `lexer.h' - (c) 2014 joseph werle - joseph.werle@gmail.com
 */

#ifndef SLY_LEXER_H
#define SLY_LEXER_H 1

#include <stdio.h>
#include "common.h"
#include "token.h"

#ifndef SLY_LEXER_MAX_BUF_SIZE
#define SLY_LEXER_MAX_BUF_SIZE 1024
#endif

typedef struct sly_lexer_s {
  const char *filename;
  char *error;
  char *source;
  char buf[SLY_LEXER_MAX_BUF_SIZE];
  char last;
  char curr;
  char peek;
  off_t offset;
  size_t length;
  int lineno;
  int col;
  int has_error;
  sly_token_t token;
} sly_lexer_t;

/**
 * Creates a new lexer with a given
 * filename and source
 */

SLY_EXTERN sly_lexer_t *
sly_lexer_new (const char *, char *);

SLY_EXTERN int
sly_lexer_scan (sly_lexer_t *);

SLY_EXTERN void
sly_lexer_reset (sly_lexer_t *);

SLY_EXTERN char
sly_lexer_next (sly_lexer_t *);

SLY_EXTERN char
sly_lexer_prev (sly_lexer_t *);

#endif
