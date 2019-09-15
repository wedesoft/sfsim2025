check_PROGRAMS = suite

check_HEADERS = munit.h test_helper.h test_list.h

suite_SOURCES = suite.c munit.c test_helper.c test_list.c
suite_CFLAGS = -I.. $(GLEW_CFLAGS) $(GL_CFLAGS) $(BOEHM_CFLAGS)
suite_LDADD = ../sfsim2025/libsfsim2025.la $(GLEW_LIBS) $(GL_LIBS) $(BOEHM_LIBS)

check-local:
	./suite
