check_PROGRAMS = suite

check_HEADERS = munit.h test_helper.h test_list.h test_matrix.h test_mechanics.h test_plane.h test_polygon.h test_quaternion.h \
								test_rigid_body.h test_vector.h

suite_SOURCES = suite.c munit.c test_helper.c test_list.c test_matrix.c test_mechanics.c test_plane.c test_polygon.c \
								test_quaternion.c test_rigid_body.c test_vector.c
suite_CFLAGS = -I.. $(GLEW_CFLAGS) $(GL_CFLAGS) -I/usr/include/gc $(BOEHM_CFLAGS)
suite_LDADD = -L/usr/lib ../sfsim2025/libsfsim2025.la $(GLEW_LIBS) $(GL_LIBS) $(BOEHM_LIBS) -lm

check-local:
	./suite
