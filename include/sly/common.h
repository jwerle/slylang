
/**
 * `common.h` - sly
 *
 * copyright (c) 2014 - joseph werle <joseph.werle@gmail.com>
 */

#ifndef SLY_COMMON_H
#define SLY_COMMON_H

#if __GNUC__ >= 4
# define SLY_EXTERN __attribute__((visibility("default")))
#else
# define SLY_EXTERN
#endif

#endif
