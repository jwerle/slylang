
/**
 * `ast.h' - (c) 2014 joseph werle - joseph.werle@gmail.com
 */

#ifndef SLY_NODE_H
#define SLY_NODE_H 1

#include <list/list.h>

/**
 * Node types
 */

#define SLY_NODE_TYPES \
  X(IDENTIFIER)        \
  X(UNDEFINED)         \
  X(GENERIC)           \
  X(LET)               \
  X(DEFINE)            \
  X(UNDEFINE)          \
  X(VAR)               \
  X(IMPORT)            \
  X(EXPORT)            \
  X(IN)                \
  X(NULL)              \
  X(FOR)               \
  X(WHILE)             \
  X(SWITCH)            \
  X(CASE)              \
  X(CASEDEFAULT)       \
  X(IF)                \
  X(ELSEIF)            \
  X(ELSE)              \
  X(RETURN)            \
  X(FREE)              \
  X(TYPEDEF)           \
  X(TYPEOF)            \
  X(BLOCK)             \
  X(EXPRESSION)        \

typedef enum {
#define X(t) SLY_NODE_##t,
  SLY_NODE_TYPES
#undef X
} sly_node_type_t;

/**
 * sly node meta data
 */

typedef struct sly_meta_s {
  sly_node_type_t type;
  int startcol;
  int endcol;
  int lineno;
} sly_meta_t;

#endif
