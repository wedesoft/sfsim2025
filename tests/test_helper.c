#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <gc.h>
#include "test_helper.h"


void* test_setup_gc(const MunitParameter params[], void* user_data) {
  GC_INIT();
}

void test_teardown_gc(void *fixture) {
  GC_gcollect();
}
