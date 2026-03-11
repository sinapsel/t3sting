#include "abort.h"
#include "params.h"

static int aborted = 0;

int __has_abort()
{
    return aborted;
}

void __reset_abort()
{
    aborted = 0;
}

void __wrap_abort()
{
    aborted++;
#if TEST_VERBOSITY > TEST_NORMAL
    fprintf(stderr, ENDL YELLOW "ABORT CALLED" NORMAL ENDL);
#endif
}

extern void __real_abort();