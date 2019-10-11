noinst_PROGRAMS = tumble

lib_LTLIBRARIES = libsfsim2025.la

pkginclude_HEADERS = coordinate.h edge.h face.h list.h matrix.h mechanics.h plane.h polygon.h quaternion.h rigid_body.h vector.h

tumble_SOURCES = tumble.c
tumble_CFLAGS = $(GLEW_CFLAGS) $(GL_CFLAGS) $(BOEHM_CFLAGS) $(SDL_CFLAGS)
tumble_LDFLAGS =
tumble_LDADD = libsfsim2025.la $(GLEW_LIBS) $(GL_LIBS) $(BOEHM_LIBS) $(SDL_LIBS) -lm

libsfsim2025_la_SOURCES = list.c matrix.c mechanics.c plane.c polygon.c quaternion.c rigid_body.c vector.c
libsfsim2025_la_CFLAGS = $(GLEW_CFLAGS) $(GL_CFLAGS) $(ATLAS_CFLAGS) $(BOEHM_CFLAGS)
libsfsim2025_la_LDFLAGS =
libsfsim2025_la_LIBADD = $(GLEW_LIBS) $(GL_LIBS) $(ATLAS_LIBS) $(BOEHM_LIBS) -lm
