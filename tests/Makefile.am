check_PROGRAMS = suite

check_HEADERS = munit.h test_helper.h test_hull.h test_list.h test_plane.h test_rigid_body.h test_vector.h

suite_SOURCES = suite.c munit.c test_helper.c test_hull.c test_list.c test_plane.c test_rigid_body.c test_vector.c
suite_CFLAGS = -I.. $(GLEW_CFLAGS) $(GL_CFLAGS) $(BOEHM_CFLAGS)
suite_LDADD = ../sfsim2025/libsfsim2025.la $(GLEW_LIBS) $(GL_LIBS) $(BOEHM_LIBS)

check-local:
	./suite
