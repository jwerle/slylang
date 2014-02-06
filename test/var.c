
/**
 * `test/var.c` - sly
 *
 * copyright (c) 2014 - joseph werle <joseph.werle@gmail.com>
 */

#include <string.h>
#include <sly/test.h>
#include <sly/value.h>
#include <sly/var.h>
#include <ok.h>

TEST(var) {
  sly_var_t *var = sly_var_new(sly_value_new((void *)32));
  t(var);
  printf("%d\n", sly_var_value(var)->as_int);
  ok("var");
  return 0;
}
