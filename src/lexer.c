
/**
 * `lexer.c` - sly
 *
 * copyright (c) 2014 - joseph werle <joseph.werle@gmail.com>
 */

#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>
#include <buffer/buffer.h>
#include <sly/lexer.h>

static int
scan_token (sly_lexer_t *, char);

/**
 * Set the current token type
 */

#define TOKEN(name)                              \
  (self->token.type = SLY_TOKEN_##name)          \

/**
 * Advance char pointer in source
 */

#define ADVANCE()                                \
  (self->last = self->source[self->offset++])    \

/**
 * Descrease char pointer in source
 */

#define DECREASE()                               \
  (self->source[--self->offset] = self->last)    \

/**
 * Sets error message and current token
 * to `SLY_TOKEN_ILLEGAL'
 */

#define ERROR(msg) {                             \
  self->error = msg;                             \
  TOKEN(ILLEGAL);                                \
}                                                \

sly_lexer_t *
sly_lexer_new (const char *filename, char *source) {
  sly_lexer_t *self = (sly_lexer_t *) malloc(sizeof(sly_lexer_t));
  if (NULL == self) return NULL;
  self->error = NULL;
  self->source = source;
  self->filename = filename;
  self->lineno = 1;
  self->col = 1;
  self->offset = 0;
  return self;
}

static int
scan_token (sly_lexer_t *self, char ch) {
  buffer_t *buffer = buffer_new_with_size(256);
  TOKEN(IDENTIFIER);
  buffer_append(buffer, &ch);
  while (isalpha(ch = ADVANCE()) || isdigit(ch) || '_' == ch) {
    buffer_append(buffer, &ch);
  }
  DECREASE();
  buffer_append(buffer, (char *) 0);

  switch (buffer->len) {

  }
  return 1;
}

int
sly_lexer_scan (sly_lexer_t *self) {
  char ch;
  // start with illegal token
  TOKEN(ILLEGAL);

  return 0;
}
