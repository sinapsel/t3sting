#pragma once
#include <stdio.h>
#include <string.h>
#include "colors.h"
#include "abort.h"
#include "params.h"


int assert_success(const char *expr, const char *file, size_t lineno);
int assert_fail(const char *expr, const char *file, size_t lineno);
int print_stats();

void inc_success();
void inc_fail();
int pop_fail();
int pop_success();

#define ASSERT(expr)                                 \
    ((!!(expr))                                      \
         ? assert_success(#expr, __FILE__, __LINE__) \
         : assert_fail(#expr, __FILE__, __LINE__))

#define EXPECT_ABORTED()                                       \
    (__has_abort() > 0) ? assert_success("abortion occured ", __FILE__, __LINE__) \
                      : assert_fail("no abortion occured", __FILE__, __LINE__)

#define EXPECT_UNABORTED()                                       \
    (__has_abort() == 0) ? assert_success("no abortion occured ", __FILE__, __LINE__) \
                      : assert_fail("abortion occured", __FILE__, __LINE__)

#define ASSERT_EQ(expr, expected)                                          \
    (((expr == expected))                                                  \
         ? assert_success(#expr(expr) " = " #expected, __FILE__, __LINE__) \
         : assert_fail(#expr " != " #expected, __FILE__, __LINE__))

#define ASSERT_STR_EQ(left, right) ASSERT(strcmp(left, right) == 0)