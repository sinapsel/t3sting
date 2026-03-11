#pragma once
#include <stdio.h>
#include "colors.h"

int __has_abort();
void __reset_abort();
extern void __real_abort();
