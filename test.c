
/**
 * `test.c` - sly
 *
 * copyright (c) 2014 - joseph werle <joseph.werle@gmail.com>
 */

#include <sly/test.h>
#include <ok.h>

int
main (void) {

  RUN(value);
  RUN(var);

  RUN(lexer);

  ok_done();
  return 0;
}
