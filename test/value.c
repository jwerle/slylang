
/**
 * `test/value.c` - sly
 *
 * copyright (c) 2014 - joseph werle <joseph.werle@gmail.com>
 */

#include <string.h>
#include <sly/test.h>
#include <sly/value.h>
#include <ok.h>

TEST(value) {
  sly_value_t *val = NULL;

  // generic
  val = sly_value_new((void *) 42);
  t(val);
  t(SLY_NODE_GENERIC == val->type);
  t(0 == strcmp("Value", val->class));
  t(42 == val->value.as_int);
  sly_value_free(val);

  val = sly_value_new("foo berries");
  t(val);
  t(SLY_NODE_GENERIC == val->type);
  t(0 == strcmp("foo berries", val->value.as_pointer));
  sly_value_free(val);

  val = sly_value_new((void *) 'a');
  t(val);
  t(SLY_NODE_GENERIC == val->type);
  t('a' == val->value.as_char);
  sly_value_free(val);

  // undefined
  val = sly_value_new(NULL);
  t(val);
  t(SLY_NODE_UNDEFINED == val->type);
  t(NULL == val->value.as_pointer);
  sly_value_free(val);

  ok("value");
  return 0;
}
