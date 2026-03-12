#include "testing.h"
#include "abort.h"

static _test *head = NULL;
static _test *tail = NULL;

void register_test(const char *name, test_func test)
{
    _test *tmp = malloc(sizeof(_test));
    tmp->name = name;
    tmp->test = test;
    tmp->next = NULL;
    if (head == NULL && tail == NULL)
    {
        head = tmp;
        tail = tmp;
        return;
    }
    tail->next = tmp;
    tail = tmp;
}

void run_test()
{
    for (_test *t = head; t != NULL; t = t->next)
    {
#if TEST_VERBOSITY >= TEST_NORMAL
        fprintf(stderr, ENDL "===TESTING CASE: %s===" ENDL ENDL, t->name);
#endif
        __reset_abort();
        t->test();
        int succs = pop_success();
        int all = pop_fail() + succs;
#if TEST_VERBOSITY >= TEST_NORMAL
        fprintf(stderr, "\n===%d of %d tests passed "
                        "%.2f%% SUCCEEDED"
                        "===\n\n",
                succs, all,
                (float)succs / (float)all * 100.0);
#else
        if (all - succs > 0)
        {
            fprintf(stderr, "Test '%s': %d failures\n", t->name, all - succs);
        }
#endif
    }
}