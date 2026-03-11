#pragma once
#include "assertions.h"
#include <stdio.h>
#include <stdlib.h>

typedef void (*test_func)();

typedef struct _test {
    const char* name;
    test_func test;
    struct _test * next;
} _test;

void register_test(const char* name, test_func test);
void run_test();

#define TEST(test_name)	\
    static void test_name##_test();	\
    static void __attribute__((constructor))	\
    __##test_name##_test() {	\
        register_test(#test_name, test_name##_test);	\
    }	\
    static void test_name##_test()

#define TEST_ENTRY_POINT	\
    int main() {	\
        run_test();	\
        return print_stats();	\
    }
