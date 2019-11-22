noinst_PROGRAMS = tumble pendulum chain collide

lib_LTLIBRARIES = libsfsim2025.la

pkginclude_HEADERS = body.h contact.h coordinate.h edge.h face.h forces.h joint.h large_matrix.h large_vector.h list.h matrix.h \
										 matrix43.h mechanics.h plane.h polygon.h quaternion.h rigid_body.h state.h vector.h world.h

libsfsim2025_la_SOURCES = contact.c joint.c large_matrix.c large_vector.c list.c matrix.c mechanics.c plane.c polygon.c quaternion.c \
													rigid_body.c state.c vector.c world.c
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

chain_SOURCES = chain.c
chain_CFLAGS = $(GLEW_CFLAGS) $(GL_CFLAGS) $(ATLAS_CFLAGS) $(BOEHM_CFLAGS) $(SDL_CFLAGS)
chain_LDFLAGS =
chain_LDADD = libsfsim2025.la $(GLEW_LIBS) $(GL_LIBS) $(BOEHM_LIBS) $(SDL_LIBS) -lm

collide_SOURCES = collide.c
collide_CFLAGS = $(GLEW_CFLAGS) $(GL_CFLAGS) $(ATLAS_CFLAGS) $(BOEHM_CFLAGS) $(SDL_CFLAGS)
collide_LDFLAGS =
collide_LDADD = libsfsim2025.la $(GLEW_LIBS) $(GL_LIBS) $(BOEHM_LIBS) $(SDL_LIBS) -lm
