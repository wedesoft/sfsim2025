noinst_PROGRAMS = tumble pendulum

lib_LTLIBRARIES = libsfsim2025.la

pkginclude_HEADERS = body_info.h contact.h coordinate.h edge.h face.h joint.h large_matrix.h large_vector.h lemke.h \
										 linear_complementary.h list.h matrix.h matrix43.h mechanics.h multibody.h plane.h polygon.h quaternion.h \
										 rigid_body.h state.h vector.h

libsfsim2025_la_SOURCES = joint.c large_matrix.c large_vector.c lemke.c linear_complementary.c list.c matrix.c mechanics.c \
													multibody.c plane.c polygon.c quaternion.c rigid_body.c vector.c
libsfsim2025_la_CFLAGS = $(GLEW_CFLAGS) $(GL_CFLAGS) $(ATLAS_CFLAGS) $(BOEHM_CFLAGS)
libsfsim2025_la_LDFLAGS =
libsfsim2025_la_LIBADD = $(GLEW_LIBS) $(GL_LIBS) -llapack_atlas $(ATLAS_LIBS) $(BOEHM_LIBS) -lm

tumble_SOURCES = tumble.c
tumble_CFLAGS = $(GLEW_CFLAGS) $(GL_CFLAGS) $(ATLAS_CFLAGS) $(BOEHM_CFLAGS) $(SDL_CFLAGS)
tumble_LDFLAGS =
tumble_LDADD = libsfsim2025.la $(GLEW_LIBS) $(GL_LIBS) $(BOEHM_LIBS) $(SDL_LIBS) -lm

pendulum_SOURCES = pendulum.c
pendulum_CFLAGS = $(GLEW_CFLAGS) $(GL_CFLAGS) $(ATLAS_CFLAGS) $(BOEHM_CFLAGS) $(SDL_CFLAGS)
pendulum_LDFLAGS =
pendulum_LDADD = libsfsim2025.la $(GLEW_LIBS) $(GL_LIBS) $(BOEHM_LIBS) $(SDL_LIBS) -lm
