
/**
 * `test.h` - sly
 *
 * copyright (c) 2014 - joseph werle <joseph.werle@gmail.com>
 */

#ifndef SLY_TEST_H
#define SLY_TEST_H

#include <stdio.h>
#include <ok.h>

static int TESTS_TOTAL;
static int TESTS_PASSED;
static int TESTS_FAILED;

#define RUN(n)                              \
  ok_expect(++TESTS_TOTAL);                 \
  if (0 != test_## n()) {                   \
    TESTS_FAILED++;                         \
  } else {                                  \
    TESTS_PASSED++;                         \
  }

#define THROW(str) fprintf(stderr, str); exit(1);
#define TEST(n) int test_## n ()
#define t(e) if (!(e)) return 1;

/**
 * A list of all the test to run
 **/

TEST(value);
TEST(var);

TEST(lexer);

#endif
