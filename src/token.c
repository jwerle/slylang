
/**
 * `token.c` - sly
 *
 * copyright (c) 2014 - joseph werle <joseph.werle@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <sly/token.h>

void
sly_token_inspect (sly_token_t *token) {
  printf("[%s] =>\n", sly_token_name(token));
  switch (token->type) {

  }
  printf("\n");
}
