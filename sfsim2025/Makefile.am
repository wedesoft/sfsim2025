lib_LTLIBRARIES = libsfsim2025.la

pkginclude_HEADERS = coordinate.h edge.h face.h hull.h list.h plane.h rigid_body.h vector.h

libsfsim2025_la_SOURCES = hull.c list.c plane.c rigid_body.c vector.c
libsfsim2025_la_CFLAGS = $(GLEW_CFLAGS) $(GL_CFLAGS) $(BOEHM_CFLAGS)
libsfsim2025_la_LDFLAGS =
libsfsim2025_la_LIBADD = $(GLEW_LIBS) $(GL_LIBS) $(BOEHM_LIBS) -lm
