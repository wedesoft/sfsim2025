lib_LTLIBRARIES = libsfsim2025.la

pkginclude_HEADERS = edge.h face.h list.h rigid_body.h vector.h plane.h

libsfsim2025_la_SOURCES = list.c rigid_body.c
libsfsim2025_la_CFLAGS = $(GLEW_CFLAGS) $(GL_CFLAGS) $(BOEHM_CFLAGS)
libsfsim2025_la_LDFLAGS =
libsfsim2025_la_LIBADD = $(GLEW_LIBS) $(GL_LIBS) $(BOEHM_LIBS) -lm
