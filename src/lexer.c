
/**
 * `lexer.c` - sly
 *
 * copyright (c) 2014 - joseph werle <joseph.werle@gmail.com>
 */

#include <math.h>
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

static int
to_hex (const char);

/**
 * Set the current token type
 */

#define TOKEN(name)                                                            \
  (self->token.type = SLY_TOKEN_##name,                                        \
   self->token.value.as_string = (char *) sly_token_name(&self->token),        \
   self->token.type)                                                           \

/**
 * Sets error message and current token
 * to `SLY_TOKEN_ILLEGAL'
 */

#define ERROR(msg) {                                                           \
  self->error = msg;                                                           \
  TOKEN(ILLEGAL);                                                              \
}                                                                              \

#define EQ(a,b) (0 == strcmp(a,b))


static int
to_hex (const char ch) {
  if (ch >= '0' && ch <= '9') return ch - '0';
  if (ch >= 'a' && ch <= 'f') return ch - ('a' + 10);
  if (ch >= 'A' && ch <= 'F') return ch - ('A' + 10);
  return -1;
}

sly_lexer_t *
sly_lexer_new (const char *filename, char *source) {
  sly_lexer_t *self = (sly_lexer_t *) malloc(sizeof(sly_lexer_t));
  if (NULL == self) return NULL;
  self->source = source;
  self->filename = filename;
  sly_lexer_reset(self);
  return self;
}

char
sly_lexer_next (sly_lexer_t *self) {
  char ch = self->source[self->offset];
  if (self->offset < self->length) {
    self->col++;
    self->offset++;
    self->last = self->curr;
    self->curr = ch;
    self->peek = self->source[self->offset];
  }
  return ch;
}

char
sly_lexer_prev (sly_lexer_t *self) {
  char ch = self->source[0];
  if (self->offset > 0) {
    ch = self->source[--self->offset];
    self->col--;
    self->last = self->curr;
    self->curr = ch;
    self->peek = self->source[self->offset + 1];
  }
  return ch;
}

static int
scan_identifier (sly_lexer_t *self, char ch) {
  char buf[256];
  int len = 0;
  // looking for identifiers
  TOKEN(IDENTIFIER);
  buf[len++] = ch;
  while (isalpha(ch = sly_lexer_next(self)) || isdigit(ch) || '_' == ch) {
    buf[len++] = ch;
  }
  buf[len] = '\0';

  switch (len) {
    // in, if
    case 2:
      if (EQ("in", buf)) { TOKEN(IN); }
      if (EQ("if", buf)) { TOKEN(IF); }
      break;

    // let, var, for
    case 3:
      if (EQ("let", buf)) { TOKEN(LET); }
      if (EQ("var", buf)) { TOKEN(VAR); }
      if (EQ("for", buf)) { TOKEN(FOR); }
      break;

    // null, case, else, free
    case 4:
      if (EQ("null", buf)) { TOKEN(NULL); }
      if (EQ("case", buf)) { TOKEN(CASE); }
      if (EQ("else", buf)) { TOKEN(ELSE); }
      if (EQ("free", buf)) { TOKEN(FREE); }
      break;

    // while, break, float
    case 5:
      if (EQ("while", buf)) { TOKEN(WHILE); }
      if (EQ("break", buf)) { TOKEN(BREAK); }
      break;

    // define, import, export, switch, return, delete
    // typeof
    case 6:
      if (EQ("define", buf)) { TOKEN(DEFINE); }
      if (EQ("import", buf)) { TOKEN(IMPORT); }
      if (EQ("export", buf)) { TOKEN(EXPORT); }
      if (EQ("switch", buf)) { TOKEN(SWITCH); }
      if (EQ("return", buf)) { TOKEN(RETURN); }
      if (EQ("delete", buf)) { TOKEN(DELETE); }
      if (EQ("typeof", buf)) { TOKEN(TYPEOF); }
      break;

    // default, typedef
    case 7:
      if (EQ("default", buf)) { TOKEN(DEFAULT); }
      if (EQ("typedef", buf)) { TOKEN(TYPEDEF); }
      break;

    // undefine
    case 8:
      if (EQ("undefine", buf)) { TOKEN(UNDEFINE); }
      break;
  }

  buffer_t *tmp = buffer_new_with_string(buf);
  self->token.value.as_string = tmp->data;
  sly_lexer_prev(self);
  free(tmp);
  return self->token.type;
}

static int
scan_string (sly_lexer_t *self, char ch) {
  char orig = ch;
  char buf[256];
  int len = 0;
  // looking or string parts
  TOKEN(STRING);
  while (orig != (ch = sly_lexer_next(self))) {
    switch (ch) {
      case '\n': self->lineno++; break;
      case '\\':
        switch ((ch = sly_lexer_next(self))) {
          case 'n': ch = '\n'; break;
          case 'r': ch = '\r'; break;
          case 't': ch = '\t'; break;
          case 'a': ch = '\a'; break;
          case 'b': ch = '\b'; break;
          case 'e': ch = '\e'; break;
          case 'f': ch = '\f'; break;
          case 'v': ch = '\v'; break;
          case 'x':
            // @TODO - handle hex
            break;
        }
        break;
    }

    buf[len++] = ch;
  }

  buf[len] = '\0';
  buffer_t *tmp = buffer_new_with_string(buf);
  self->token.value.as_string = tmp->data;
  sly_lexer_prev(self);
  free(tmp);
  return self->token.type;
}

static int
scan_number (sly_lexer_t *self, char ch) {
  buffer_t *tmp = NULL;
  char buf[1024];
  int len = 0;
  int num = 0;
  int f = 1; // factor
  int exponent = 0;
  int is_positive = 1;
  int is_float = 0;

  // they are all ints until we say otherwise
  TOKEN(INT);
  switch (ch) {
    case '0': goto parse_hex;
    case 'u': goto parse_unicode;
    case '+': case '-': goto parse_exponent;
    default: goto parse_int;
  }

parse_hex:
  TOKEN(HEX);
  buf[len++] = ch;
  buf[len++] = ch = sly_lexer_next(self);
  if ('x' == ch) {
    if (isxdigit(ch = sly_lexer_next(self))) {
      do {
        buf[len++] = ch;
        if (!isxdigit(ch)) {
          ERROR("Unexpected token");
          return TOKEN(ILLEGAL);
        }
        num = num << 4 | to_hex(ch);
      } while (((ch = sly_lexer_next(self))) && (
               '\n' != ch && '\t' != ch && '\r' != ch &&
               '[' != ch && ']' != ch && '{' != ch &&
               '}' != ch && '(' != ch && ')' != ch));
    } else {
      ERROR("Unexpected");
      return TOKEN(ILLEGAL);
    }
  } else {
    buf[--len] = '\0';
    goto parse_int;
  }

  self->token.value.as_int = num;
  buf[len] = '\0';
  tmp = buffer_new_with_string(buf);
  self->token.value.as_string = tmp->data;
  free(tmp);
  sly_lexer_prev(self);
  return self->token.type;

parse_unicode: // @TODO
  return TOKEN(UNICODE);

parse_int:
  TOKEN(INT);

  while (isdigit(ch) || '.' == ch || 'e' == ch || 'E' == ch) {
    if ('.' == ch) goto parse_float;
    if ('e' == ch || 'E' == ch) goto parse_exponent;
    buf[len++] = ch;
    num = (num * 10) + (ch - '0');
    ch = sly_lexer_next(self);
  }
  self->token.value.as_int = num;
  buf[len] = '\0';
  tmp = buffer_new_with_string(buf);
  self->token.value.as_string = tmp->data;
  free(tmp);
  sly_lexer_prev(self);
  return self->token.type;

parse_float:
  TOKEN(FLOAT);
  is_float = 1;
    buf[len++] = ch;
  while (isdigit(ch = sly_lexer_next(self)) || '.' == ch || 'e' == ch || 'E' == ch) {
    if ('e' == ch || 'E' == ch) goto parse_exponent;
    buf[len++] = ch;
    num = (num * 10) + (ch - '0');
    f = f * 10;
  }
  self->token.value.as_float = (float) num / f;
  buf[len] = '\0';
  tmp = buffer_new_with_string(buf);
  self->token.value.as_string = tmp->data;
  free(tmp);
  sly_lexer_prev(self);
  return self->token.type;

parse_exponent:
  buf[len++] = ch;
  // account for positive and negative exponents
  while (isdigit(ch = sly_lexer_next(self)) || '+' == ch || '-' == ch) {
    buf[len++] = ch;
    if ('-' == ch) {
      is_positive = 0;
      continue;
    }

    exponent = (exponent * 10) + (ch - '0');
  }

  if (!is_positive) {
    exponent = exponent * -1;
  }
  if (is_float) {
    self->token.value.as_float = pow(10, exponent) * ((float) num / f);
  } else {
    self->token.value.as_int = pow(10, exponent) * num;
  }

  buf[len] = '\0';
  tmp = buffer_new_with_string(buf);
  self->token.value.as_string = tmp->data;
  free(tmp);
  sly_lexer_prev(self);
  return self->token.type;
}


int
sly_lexer_scan (sly_lexer_t *self) {
  char ch = 0;
  // start with an illegal token
  TOKEN(ILLEGAL);

scan:
  switch (ch = sly_lexer_next(self)) {
    // whitespace
    case ' ': case '\t':
      goto scan;
      break;

    // new lines
    case '\r': case '\n':
      self->lineno++;
      self->col = 1;
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
    case '^':
      switch (sly_lexer_next(self)) {
        case '=': return TOKEN(OP_BITWISE_XOR_ASSIGN);
        default: return TOKEN(OP_BITWISE_XOR);
      }

    case '!':
      switch (ch = sly_lexer_next(self)) {
        case '=': return TOKEN(OP_NEQ);
        default:
          sly_lexer_prev(self);
          return TOKEN(OP_NEGATE);
      }

    case '=':
      switch (sly_lexer_next(self)) {
        case '<': return TOKEN(OP_LTE);
        case '=': return TOKEN(OP_EQ);
        default:
          sly_lexer_prev(self);
          return TOKEN(OP_ASSIGN);
      }

    case '&':
      switch (sly_lexer_next(self)) {
        case '=': return TOKEN(OP_BITWISE_AND_ASSIGN);
        case '&': return TOKEN(OP_AND);
        default:
          sly_lexer_prev(self);
          return TOKEN(OP_BITWISE_AND);
      }

    case '|':
      switch (sly_lexer_next(self)) {
        case '|': return TOKEN(OP_OR);
        case '=': return TOKEN(OP_BITWISE_OR_ASSIGN);
        default:
          sly_lexer_prev(self);
          return TOKEN(OP_BITWISE_OR);
      }

    case '+':
      switch (ch = sly_lexer_next(self)) {
        case '+': return TOKEN(OP_INCREMENT);
        case '=': return TOKEN(OP_PLUS_ASSIGN);
        default:
          if (isdigit(ch)) {
            return scan_number(self, ch);
          } else {
            sly_lexer_prev(self);
            return TOKEN(OP_PLUS);
          }
      }

    case '-':
      switch (ch = sly_lexer_next(self)) {
        case '-': return TOKEN(OP_DECREMENT);
        case '=': return TOKEN(OP_MINUS_ASSIGN);
        default:
          if (isdigit(ch)) {
            return scan_number(self, ch);
          } else {
            sly_lexer_prev(self);
            return TOKEN(OP_MINUS);
          }
      }

    case '*':
      switch (sly_lexer_next(self)) {
        case '=': return TOKEN(OP_MULTIPLY_ASSIGN);
        default:
          sly_lexer_prev(self);
          return TOKEN(OP_MULTIPLY);
      }

    case '/':
      switch (sly_lexer_next(self)) {
        case '=': return TOKEN(OP_DIVIDE_ASSIGN);
        case '/':
           while ('\n' != (ch = sly_lexer_next(self)) && ch);
           sly_lexer_prev(self);
           goto scan;
        default:
          sly_lexer_prev(self);
          return TOKEN(OP_DIVIDE);
      }

    case '>':
      switch (sly_lexer_next(self)) {
        case '=': return TOKEN(OP_GTE);
        case '>':
          switch (sly_lexer_next(self)) {
            case '>':
              switch (sly_lexer_next(self)) {
                case '=': return TOKEN(OP_BITWISE_UNSIGNED_SHIFT_RIGHT_ASSIGN);
              }
              return TOKEN(OP_BITWISE_UNSIGNED_SHIFT_RIGHT);
          }
          sly_lexer_prev(self);
          return TOKEN(OP_BITWISE_SHIFT_RIGHT);
      }
      sly_lexer_prev(self);
      return TOKEN(OP_GT);

    case '<':
      switch (sly_lexer_next(self)) {
        case '=': return TOKEN(OP_LTE);
        case '<':
          switch (sly_lexer_next(self)) {
            case '=': return TOKEN(OP_BITWISE_SHIFT_LEFT_ASSIGN);
          }
          return TOKEN(OP_BITWISE_SHIFT_LEFT);
      }
      sly_lexer_prev(self);
      return TOKEN(OP_LT);

    case 0:
      return TOKEN(EOF);

    default:
      if ('_' == ch || isalpha(ch)) return scan_identifier(self, ch);
      if ('.' == ch || isdigit(ch)) return scan_number(self, ch);
      ERROR("Unexpected token");
  }

  return TOKEN(ILLEGAL);
}

void
sly_lexer_reset (sly_lexer_t *self) {
  self->error = NULL;
  self->lineno = 1;
  self->col = 1;
  self->offset = 0;
  self->last = 0;
  self->length = strlen(self->source);
}
