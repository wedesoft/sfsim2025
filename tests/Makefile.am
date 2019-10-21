check_PROGRAMS = suite

check_HEADERS = munit.h test_contact.h test_helper.h test_large_matrix.h test_large_vector.h test_linear_complementary.h \
								test_list.h test_matrix.h test_matrix43.h test_mechanics.h test_multibody.h test_plane.h test_polygon.h \
								test_quaternion.h test_rigid_body.h test_state.h test_vector.h

suite_SOURCES = suite.c munit.c test_helper.c test_contact.c test_large_matrix.c test_large_vector.c test_linear_complementary.c \
								test_list.c test_matrix.c test_matrix43.c test_mechanics.c test_multibody.c test_plane.c test_polygon.c \
								test_quaternion.c test_state.c test_rigid_body.c test_vector.c
suite_CFLAGS = -I.. $(GLEW_CFLAGS) $(GL_CFLAGS) $(ATLAS_CFLAGS) $(BOEHM_CFLAGS)
suite_LDADD = ../sfsim2025/libsfsim2025.la $(GLEW_LIBS) $(GL_LIBS) $(ATLAS_LIBS) $(BOEHM_LIBS) -lm

check-local:
	./suite
