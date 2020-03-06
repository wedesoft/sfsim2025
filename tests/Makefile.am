check_DATA = black.png color.png indices.png zeros.raw data.raw cache-2-5-3.png cache-2-5-4.png cache-2-5-7.png \
						 data.raw indices.raw zeros.raw cache-2-5-3.raw cache-2-5-4.raw cache-2-5-7.raw

check_PROGRAMS = suite

check_HEADERS = munit.h test_cache.h test_contact.h test_elevation.h test_helper.h test_force.h test_friction.h test_image.h \
								test_joint.h test_large_matrix.h test_large_vector.h test_list.h test_map.h test_matrix.h test_mechanics.h \
								test_plane.h test_polygon.h test_projection.h test_quaternion.h test_hull.h test_state.h test_vector.h test_world.h

suite_SOURCES = suite.c munit.c test_cache.c test_contact.c test_elevation.c test_force.c test_friction.c test_helper.c test_image.c \
								test_joint.c test_large_matrix.c test_large_vector.c test_list.c test_map.c test_matrix.c test_mechanics.c \
								test_plane.c test_polygon.c test_projection.c test_quaternion.c test_state.c test_hull.c test_vector.c test_world.c
suite_CFLAGS = -I.. $(GLEW_CFLAGS) $(GL_CFLAGS) $(ATLAS_CFLAGS) $(BOEHM_CFLAGS)
suite_LDADD = ../sfsim2025/libsfsim2025.la $(GLEW_LIBS) $(GL_LIBS) $(ATLAS_LIBS) $(BOEHM_LIBS) -lm

EXTRA_DIST = black.png color.png indices.png zeros.raw data.raw cache-2-5-3.png cache-2-5-4.png cache-2-5-7.png \
						 data.raw indices.raw zeros.raw cache-2-5-3.raw cache-2-5-4.raw cache-2-5-7.raw

check-local:
	./suite
