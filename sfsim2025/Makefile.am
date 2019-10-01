lib_LTLIBRARIES = libsfsim2025.la

pkginclude_HEADERS = coordinate.h edge.h face.h list.h matrix.h mechanics.h plane.h polygon.h quaternion.h rigid_body.h vector.h

libsfsim2025_la_SOURCES = list.c mechanics.c plane.c polygon.c rigid_body.c vector.c
libsfsim2025_la_CFLAGS = $(GLEW_CFLAGS) $(GL_CFLAGS) $(BOEHM_CFLAGS)
libsfsim2025_la_LDFLAGS =
libsfsim2025_la_LIBADD = $(GLEW_LIBS) $(GL_LIBS) $(BOEHM_LIBS) -lm
