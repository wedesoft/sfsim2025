#pragma once
#include "munit.h"


void* test_setup_gc(const MunitParameter params[], void* user_data);

void test_teardown_gc(void *fixture);
