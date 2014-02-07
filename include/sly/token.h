
/**
 * `token.h' - (c) 2014 joseph werle - joseph.werle@gmail.com
 */

#ifndef SLY_TOKEN_H
#define SLY_TOKEN_H 1

#include "common.h"

/**
 * Syntax tokens
 */

#define SLY_TOKENS                                \
  X(ILLEGAL, "illegal")                           \
  X(EOF, "eof")                                   \
  X(IDENTIFIER, "identifier")                     \
  X(UNDEFINED, "undefined")                       \
  X(NULL, "null")                                 \
  X(UNDEFINE, "undefine")                         \
  X(DEFINE, "define")                             \
  X(LET, "let")                                   \
  X(VAR, "var")                                   \
  X(IMPORT, "import")                             \
  X(EXPORT, "export")                             \
  X(IN, "in")                                     \
  X(FOR, "for")                                   \
  X(WHILE, "while")                               \
  X(SWITCH, "switch")                             \
  X(CASE, "case")                                 \
  X(BREAK, "break")                               \
  X(DEFAULT, "default")                           \
  X(IF, "if")                                     \
  X(ELSE, "else")                                 \
  X(RETURN, "return")                             \
  X(FREE, "free")                                 \
  X(DELETE, "delete")                             \
  X(TYPEDEF, "typedef")                           \
  X(TYPEOF, "typeof")                             \
  X(STRING, "string")                             \
  X(INT, "int")                                   \
  X(FLOAT, "float")                               \
  X(HEX, "hex")                                   \
  X(UNICODE, "unicode")                           \
  X(LBRACE, "{")                                  \
  X(RBRACE, "}")                                  \
  X(LBRACKET, "[")                                \
  X(RBRACKET, "]")                                \
  X(LPAREN, "(")                                  \
  X(RPAREN, ")")                                  \
  X(SEMICOLON, ";")                               \
  X(COLON, ":")                                   \
  X(QMARK, "?")                                   \
  X(COMMA, ",")                                   \
  X(OP_DOT, ".")                                  \
  X(OP_NEGATE, "!")                               \
  X(OP_PLUS, "+")                                 \
  X(OP_INCREMENT, "++")                           \
  X(OP_MINUS, "-")                                \
  X(OP_DECREMENT, "--")                           \
  X(OP_MULTIPLY, "*")                             \
  X(OP_DIVIDE, "/")                               \
  X(OP_MODULO, "%")                               \
  X(OP_GT, ">")                                   \
  X(OP_LT, "<")                                   \
  X(OP_GTE, ">=")                                 \
  X(OP_LTE, "<=")                                 \
  X(OP_EQ, "==")                                  \
  X(OP_NEQ, "!=")                                 \
  X(OP_AND, "&&")                                 \
  X(OP_OR, "||")                                  \
  X(OP_ASSIGN, "=")                               \
  X(OP_PLUS_ASSIGN, "+=")                         \
  X(OP_MINUS_ASSIGN, "-=")                        \
  X(OP_MULTIPLY_ASSIGN, "*=")                     \
  X(OP_DIVIDE_ASSIGN, "/=")                       \
  X(OP_BITWISE_AND, "&")                          \
  X(OP_BITWISE_OR, "|")                           \
  X(OP_BITWISE_XOR, "^")                          \
  X(OP_BITWISE_NOT, "~")                          \
  X(OP_BITWISE_SHIFT_LEFT, "<<")                  \
  X(OP_BITWISE_SHIFT_RIGHT, ">>")                 \
  X(OP_BITWISE_UNSIGNED_SHIFT_RIGHT, ">>>")       \

/**
 * Token enum type
 */

typedef enum {
#define X(token, name) SLY_TOKEN_##token,
  SLY_TOKENS
#undef X
} sly_token_type_t;

/**
 * Token type string names
 */

static char *sly_token_names[] = {
#define X(token, name) name,
  SLY_TOKENS
#undef X
};

/**
 * Token coerce type
 */

typedef struct sly_token_coerce_s {
  char *as_string;
  float as_float;
  int as_int;
} sly_token_coerce_t;

/**
 * Token type
 */

typedef struct sly_token_s {
  sly_token_type_t type;
  int length;
  sly_token_coerce_t value;
} sly_token_t;

/**
 * Returns a string representing the
 * token type
 */

static inline const char *
sly_token_name (sly_token_t *token) {
  return sly_token_names[token->type];
}

/**
 * Prints information about the
 * token provided to stdout
 */

SLY_EXTERN void
sly_token_inspect (sly_token_t *);

#endif
