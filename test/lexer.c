
/**
 * `test/ast.c` - sly
 *
 * copyright (c) 2014 - joseph werle <joseph.werle@gmail.com>
 */

#include <sly/test.h>
#include <sly/lexer.h>
#include <ok.h>

TEST(lexer) {
  sly_lexer_t *lexer = NULL;

  lexer = sly_lexer_new("test", "`");
  t(lexer);
  t(SLY_TOKEN_ILLEGAL == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "");
  t(lexer);
  t(SLY_TOKEN_EOF == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "null");
  t(lexer);
  t(SLY_TOKEN_NULL == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "undefine");
  t(lexer);
  t(SLY_TOKEN_UNDEFINE == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "define CONSTANT 1234");
  t(lexer);
  t(SLY_TOKEN_DEFINE == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "let noop () {}");
  t(lexer);
  t(SLY_TOKEN_LET == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "var foo = 123;");
  t(lexer);
  t(SLY_TOKEN_VAR == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "import {foo} in \"bar\"");
  t(lexer);
  t(SLY_TOKEN_IMPORT == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "export var bar = '\"foo\"");
  t(lexer);
  t(SLY_TOKEN_EXPORT == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "in \"stdio\"");
  t(lexer);
  t(SLY_TOKEN_IN == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "for (var i = 0; i < 10; ++i) {}");
  t(lexer);
  t(SLY_TOKEN_FOR == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "while (true) {}");
  t(lexer);
  t(SLY_TOKEN_WHILE == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "switch (1) { case 1: break; }");
  t(lexer);
  t(SLY_TOKEN_SWITCH == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "case 42: break;");
  t(lexer);
  t(SLY_TOKEN_CASE == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "break;");
  t(lexer);
  t(SLY_TOKEN_BREAK == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "default: print(123);");
  t(lexer);
  t(SLY_TOKEN_DEFAULT == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "if (true) { return false; }");
  t(lexer);
  t(SLY_TOKEN_IF == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "else if (false) { return true; }");
  t(lexer);
  t(SLY_TOKEN_ELSE == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "return 1;");
  t(lexer);
  t(SLY_TOKEN_RETURN == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "free foo;");
  t(lexer);
  t(SLY_TOKEN_FREE == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "delete bar;");
  t(lexer);
  t(SLY_TOKEN_DELETE == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "typedef Person {};");
  t(lexer);
  t(SLY_TOKEN_TYPEDEF == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "typeof joe;");
  t(lexer);
  t(SLY_TOKEN_TYPEOF == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "\"foo bar\"");
  t(lexer);
  t(SLY_TOKEN_STRING == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "32");
  t(lexer);
  t(SLY_TOKEN_INT == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "-32");
  t(lexer);
  t(SLY_TOKEN_INT == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "00000001");
  t(lexer);
  t(SLY_TOKEN_INT == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "10e10");
  t(lexer);
  t(SLY_TOKEN_INT == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "20E50");
  t(lexer);
  t(SLY_TOKEN_INT == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "0.45e99");
  t(lexer);
  t(SLY_TOKEN_FLOAT == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "-123E456");
  t(lexer);
  t(SLY_TOKEN_INT == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "-123.456E789");
  t(lexer);
  t(SLY_TOKEN_FLOAT == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "-0x00000004");
  t(lexer);
  t(SLY_TOKEN_HEX == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "+0x0008");
  t(lexer);
  t(SLY_TOKEN_HEX == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "-0x04.0");
  t(lexer);
  t(SLY_TOKEN_ILLEGAL == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "64.12345");
  t(lexer);
  t(SLY_TOKEN_FLOAT == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "0x4f");
  t(lexer);
  t(SLY_TOKEN_HEX == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "{");
  t(lexer);
  t(SLY_TOKEN_LBRACE == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "}");
  t(lexer);
  t(SLY_TOKEN_RBRACE == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "[");
  t(lexer);
  t(SLY_TOKEN_LBRACKET == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "]");
  t(lexer);
  t(SLY_TOKEN_RBRACKET == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "(");
  t(lexer);
  t(SLY_TOKEN_LPAREN == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", ")");
  t(lexer);
  t(SLY_TOKEN_RPAREN == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", ";");
  t(lexer);
  t(SLY_TOKEN_SEMICOLON == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", ":");
  t(lexer);
  t(SLY_TOKEN_COLON == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "?");
  t(lexer);
  t(SLY_TOKEN_QMARK == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", ",");
  t(lexer);
  t(SLY_TOKEN_COMMA == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", ".");
  t(lexer);
  t(SLY_TOKEN_OP_DOT == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "!");
  t(lexer);
  t(SLY_TOKEN_OP_NEGATE == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "+");
  t(lexer);
  t(SLY_TOKEN_OP_PLUS == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "++");
  t(lexer);
  t(SLY_TOKEN_OP_INCREMENT == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "-");
  t(lexer);
  t(SLY_TOKEN_OP_MINUS == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "--");
  t(lexer);
  t(SLY_TOKEN_OP_DECREMENT == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "*");
  t(lexer);
  t(SLY_TOKEN_OP_MULTIPLY == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "/");
  t(lexer);
  t(SLY_TOKEN_OP_DIVIDE == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "%");
  t(lexer);
  t(SLY_TOKEN_OP_MODULO == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", ">");
  t(lexer);
  t(SLY_TOKEN_OP_GT == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "<");
  t(lexer);
  t(SLY_TOKEN_OP_LT == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", ">=");
  t(lexer);
  t(SLY_TOKEN_OP_GTE == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "=<");
  t(lexer);
  t(SLY_TOKEN_OP_LTE == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "==");
  t(lexer);
  t(SLY_TOKEN_OP_EQ == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "!=");
  t(lexer);
  t(SLY_TOKEN_OP_NEQ == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "&&");
  t(lexer);
  t(SLY_TOKEN_OP_AND == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "||");
  t(lexer);
  t(SLY_TOKEN_OP_OR == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "=");
  t(lexer);
  t(SLY_TOKEN_OP_ASSIGN == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "+=");
  t(lexer);
  t(SLY_TOKEN_OP_PLUS_ASSIGN == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "-=");
  t(lexer);
  t(SLY_TOKEN_OP_MINUS_ASSIGN == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "*=");
  t(lexer);
  t(SLY_TOKEN_OP_MULTIPLY_ASSIGN == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "/=");
  t(lexer);
  t(SLY_TOKEN_OP_DIVIDE_ASSIGN == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "&");
  t(lexer);
  t(SLY_TOKEN_OP_BITWISE_AND == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "|");
  t(lexer);
  t(SLY_TOKEN_OP_BITWISE_OR == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "^");
  t(lexer);
  t(SLY_TOKEN_OP_BITWISE_XOR == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "~");
  t(lexer);
  t(SLY_TOKEN_OP_BITWISE_NOT == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", "<<");
  t(lexer);
  t(SLY_TOKEN_OP_BITWISE_SHIFT_LEFT == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", ">>");
  t(lexer);
  t(SLY_TOKEN_OP_BITWISE_SHIFT_RIGHT == sly_lexer_scan(lexer));

  lexer = sly_lexer_new("test", ">>>");
  t(lexer);
  t(SLY_TOKEN_OP_BITWISE_UNSIGNED_SHIFT_RIGHT == sly_lexer_scan(lexer));

  ok("lexer");
  return 0;
}
