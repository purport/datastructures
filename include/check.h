#pragma once

#include <stdio.h>

#ifndef CHECK_PRINT_PASSES
#define CHECK_PRINT_PASSES 0
#endif
#define CHECK_START() \
  u32 failed = 0;     \
  u32 total = 0
#define CHECK_EARLY_EXIT() \
  if (failed) goto CHECK_EARLY_EXIT_POINT
#define CHECK_END()                                                    \
  CHECK_EARLY_EXIT_POINT:                                              \
  if (failed) {                                                        \
    printf("\x1b[31m[FAIL]\x1b[0m %d/%d tests\n", failed, total); \
  } else {                                                             \
    if (CHECK_PRINT_PASSES) {                                          \
      printf("\x1b[32m[PASS]\x1b[0m %d tests\n", total);          \
    }                                                                  \
  }                                                                    \
  return (s32)failed

#define STRINGIZE(x)  STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING   STRINGIZE(__LINE__)

#define CHECK(lhs, op, rhs)                                                                     \
  do {                                                                                          \
    ++total;                                                                                    \
    if ((lhs)op(rhs)) {                                                                         \
      if (CHECK_PRINT_PASSES) {                                                                 \
        printf(__FILE__ ":" LINE_STRING ":1: \x1b[32m[PASS]\x1b[0m " #lhs " " #op " " #rhs \
                             "\n");                                                             \
      }                                                                                         \
    } else {                                                                                    \
      printf(__FILE__ ":" LINE_STRING ":1: \x1b[31m[FAIL]\x1b[0m " #lhs " " #op " " #rhs   \
                           "\n");                                                               \
      ++failed;                                                                                 \
    }                                                                                           \
  } while (0)

#define CHECK_TRUE(lhs) CHECK(lhs, ==, true)