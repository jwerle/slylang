
/**
 * `lexer.c` - sly
 *
 * copyright (c) 2014 - joseph werle <joseph.werle@gmail.com>
 */

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <buffer/buffer.h>
#include <sly/lexer.h>

static int
scan_identifier (sly_lexer_t *, char);

static int
scan_string (sly_lexer_t *, char);

static int
scan_number (sly_lexer_t *, char);

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

#define EQ(a,b) (0 == strcmp(a,b))

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
scan_identifier (sly_lexer_t *self, char ch) {
  buffer_t *buf = buffer_new_with_size(256);
  // looking for identifiers
  TOKEN(IDENTIFIER);
  buffer_append(buf, &ch);
  while (isalpha(ch = ADVANCE()) || isdigit(ch) || '_' == ch) {
    buffer_append(buf, &ch);
  }
  DECREASE();
  buffer_append(buf, (char *) 0);

  switch (buf->len) {
    // in, if
    case 2:
      if (EQ("in", buf->data)) return TOKEN(IN);
      if (EQ("if", buf->data)) return TOKEN(IF);
      break;

    // let, var, for
    case 3:
      if (EQ("let", buf->data)) return TOKEN(LET);
      if (EQ("var", buf->data)) return TOKEN(VAR);
      if (EQ("for", buf->data)) return TOKEN(FOR);
      break;

    // null, case, else, free
    case 4:
      if (EQ("null", buf->data)) return TOKEN(NULL);
      if (EQ("case", buf->data)) return TOKEN(CASE);
      if (EQ("else", buf->data)) return TOKEN(ELSE);
      if (EQ("free", buf->data)) return TOKEN(FREE);
      break;

    // while
    case 5:
      if (EQ("while", buf->data)) return TOKEN(WHILE);
      break;

    // define, import, export, switch, return, delete
    // typeof
    case 6:
      if (EQ("define", buf->data)) return TOKEN(DEFINE);
      if (EQ("import", buf->data)) return TOKEN(IMPORT);
      if (EQ("export", buf->data)) return TOKEN(EXPORT);
      if (EQ("switch", buf->data)) return TOKEN(SWITCH);
      if (EQ("return", buf->data)) return TOKEN(RETURN);
      if (EQ("delete", buf->data)) return TOKEN(DELETE);
      if (EQ("typeof", buf->data)) return TOKEN(TYPEOF);
      break;

    // default, typedef
    case 7:
      if (EQ("default", buf->data)) return TOKEN(DEFAULT);
      if (EQ("typedef", buf->data)) return TOKEN(TYPEDEF);
      break;

    // undefine
    case 8:
      if (EQ("undefine", buf->data)) return TOKEN(UNDEFINE);
      break;
  }

  self->token.value.as_string = buf->data;
  buffer_free(buf);
  return 1;
}

static int
scan_string (sly_lexer_t *self, char ch) {
  char orig = ch;
  buffer_t *buf = buffer_new_with_size(256);
  // looking or string parts
  TOKEN(STRING);
  while (orig != (ch = ADVANCE())) {
    switch (ch) {
      case '\n':
        self->lineno++;
        break;

      case '\\':
        switch ((ch = ADVANCE())) {
          case 'n':
            ch = '\n';
            break;

          case 'r':
            ch = '\r';
            break;

          case 't':
            ch = '\t';
            break;

          case 'a':
            ch = '\a';
            break;

          case 'b':
            ch = '\b';
            break;

          case 'e':
            ch = '\e';
            break;

          case 'f':
            ch = '\f';
            break;

          case 'v':
            ch = '\v';
            break;

          case 'x':
            // @TODO - handle hex
            break;
        }

        break;
    }

    buffer_append(buf, &ch);
  }

  buffer_append(buf, (char *) 0);
  self->token.value.as_string = buf->data;
  buffer_free(buf);
  return 1;
}

static int
scan_number (sly_lexer_t *self, char ch) {
  int num = 0;
  TOKEN(INT);

  switch (ch) {
    case '0': goto _hex;
    default: goto _int;
  }

_hex: // @TODO
  return 1;

_int:
  return 1;

_float:

  return 1;
}


int
sly_lexer_scan (sly_lexer_t *self) {
  char ch = 0;
  // start with an illegal token
  TOKEN(ILLEGAL);

scan:
  switch (ch = ADVANCE()) {
    // whitespace
    case ' ': case '\t':
      goto scan;
      break;

    // new lines
    case '\r': case '\n':
      self->lineno++;
      goto scan;
      break;

    // quoted strings
    case '"': case '\'':
      return scan_string(self, ch);

    case '{': return TOKEN(LBRACE);
    case '}': return TOKEN(RBRACE);
    case '[': return TOKEN(LBRACKET);
    case ']': return TOKEN(RBRACKET);
    case '(': return TOKEN(LPAREN);
    case ')': return TOKEN(RPAREN);

    case ';': return TOKEN(SEMICOLON);
    case ':': return TOKEN(COLON);
    case '?': return TOKEN(QMARK);
    case ',': return TOKEN(COMMA);

    case '.': return TOKEN(OP_DOT);
    case '%': return TOKEN(OP_MODULO);
    case '~': return TOKEN(OP_BITWISE_NOT);
    case '^': return TOKEN(OP_BITWISE_XOR);

    case '!':
      switch (ADVANCE()) {
        case '=': return TOKEN(OP_NEQ);
        default: DECREASE(); return TOKEN(OP_NEGATE);
      }

    case '=':
      switch (ADVANCE()) {
        case '=': return TOKEN(OP_EQ);
        default: DECREASE(); return TOKEN(OP_ASSIGN);
      }

    case '&':
      switch (ADVANCE()) {
        case '&': return TOKEN(OP_AND);
        default: DECREASE(); return TOKEN(OP_BITWISE_AND);
      }

    case '|':
      switch (ADVANCE()) {
        case '|': return TOKEN(OP_OR);
        default: DECREASE(); return TOKEN(OP_BITWISE_OR);
      }

    case '+':
      switch (ADVANCE()) {
        case '+': return TOKEN(OP_INCREMENT);
        case '=': return TOKEN(OP_PLUS_ASSIGN);
        default: DECREASE(); return TOKEN(OP_PLUS);
      }

    case '-':
      switch (ADVANCE()) {
        case '-': return TOKEN(OP_DECREMENT);
        case '=': return TOKEN(OP_MINUS_ASSIGN);
        default: DECREASE(); return TOKEN(OP_MINUS);
      }

    case '*':
      switch (ADVANCE()) {
        case '=': return TOKEN(OP_MULTIPLY_ASSIGN);
        default: DECREASE(); return TOKEN(OP_MULTIPLY);
      }

    case '/':
      switch (ADVANCE()) {
        case '=': return TOKEN(OP_DIVIDE_ASSIGN);
        case '/':
           while ('\n' != (ch = ADVANCE()) && ch) ; DECREASE();
           goto scan;
        default: DECREASE(); return TOKEN(OP_DIVIDE);
      }

    case '>':
      switch (ADVANCE()) {
        case '=': return TOKEN(OP_GTE);
        case '>':
          switch (ADVANCE()) {
            case '>': return TOKEN(OP_BITWISE_UNSIGHED_SHIFT_RIGHT);
            default: DECREASE(); return TOKEN(OP_BITWISE_SHIFT_RIGHT);
          }

        default: DECREASE(); return TOKEN(OP_GT);
      }

    case '<':
      switch (ADVANCE()) {
        case '=': return TOKEN(OP_LTE);
        case '<': return TOKEN(OP_BITWISE_SHIFT_LEFT);
        default: DECREASE(); return TOKEN(OP_LT);
      }

    case 0:
      TOKEN(EOF);
      break;

    default:
      if ('_' == ch || isalpha(ch)) return scan_identifier(self, ch);
      if ('.' == ch || isdigit(ch)) return scan_number(self, ch);
      ERROR("Unexpcted token");
  }

  return 0;
}
