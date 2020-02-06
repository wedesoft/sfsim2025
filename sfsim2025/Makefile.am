noinst_PROGRAMS = tumble pendulum chain collide spring gear wheel car icosahedron maptiles scale50

lib_LTLIBRARIES = libsfsim2025.la

pkginclude_HEADERS = body.h contact.h coordinate.h edge.h face.h force.h friction.h image.h joint.h large_matrix.h large_vector.h \
										 list.h matrix.h matrix43.h mechanics.h plane.h polygon.h quaternion.h hull.h state.h vector.h world.h

libsfsim2025_la_SOURCES = contact.c force.c friction.c image.c joint.c large_matrix.c large_vector.c list.c matrix.c mechanics.c \
													plane.c polygon.c quaternion.c hull.c state.c vector.c world.c
libsfsim2025_la_CFLAGS = $(GLEW_CFLAGS) $(GL_CFLAGS) $(MAGICK_CFLAGS) $(ATLAS_CFLAGS) $(BOEHM_CFLAGS)
libsfsim2025_la_LDFLAGS =
libsfsim2025_la_LIBADD = $(GLEW_LIBS) $(GL_LIBS) $(MAGICK_LIBS) -llapack_atlas $(ATLAS_LIBS) $(BOEHM_LIBS) -lm

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

spring_SOURCES = spring.c
spring_CFLAGS = $(GLEW_CFLAGS) $(GL_CFLAGS) $(ATLAS_CFLAGS) $(BOEHM_CFLAGS) $(SDL_CFLAGS)
spring_LDFLAGS =
spring_LDADD = libsfsim2025.la $(GLEW_LIBS) $(GL_LIBS) $(BOEHM_LIBS) $(SDL_LIBS) -lm

gear_SOURCES = gear.c
gear_CFLAGS = $(GLEW_CFLAGS) $(GL_CFLAGS) $(ATLAS_CFLAGS) $(BOEHM_CFLAGS) $(SDL_CFLAGS)
gear_LDFLAGS =
gear_LDADD = libsfsim2025.la $(GLEW_LIBS) $(GL_LIBS) $(BOEHM_LIBS) $(SDL_LIBS) -lm

wheel_SOURCES = wheel.c
wheel_CFLAGS = $(GLEW_CFLAGS) $(GL_CFLAGS) $(ATLAS_CFLAGS) $(BOEHM_CFLAGS) $(SDL_CFLAGS)
wheel_LDFLAGS =
wheel_LDADD = libsfsim2025.la $(GLEW_LIBS) $(GL_LIBS) $(BOEHM_LIBS) $(SDL_LIBS) -lm

car_SOURCES = car.c
car_CFLAGS = $(GLEW_CFLAGS) $(GL_CFLAGS) $(ATLAS_CFLAGS) $(BOEHM_CFLAGS) $(SDL_CFLAGS)
car_LDFLAGS =
car_LDADD = libsfsim2025.la $(GLEW_LIBS) $(GL_LIBS) $(BOEHM_LIBS) $(SDL_LIBS) -lm

icosahedron_SOURCES = icosahedron.c
icosahedron_CFLAGS = $(GLEW_CFLAGS) $(GL_CFLAGS) $(ATLAS_CFLAGS) $(BOEHM_CFLAGS) $(SDL_CFLAGS)
icosahedron_LDFLAGS =
icosahedron_LDADD = libsfsim2025.la $(GLEW_LIBS) $(GL_LIBS) $(BOEHM_LIBS) $(SDL_LIBS) -lm

maptiles_SOURCES = maptiles.c
maptiles_CFLAGS = $(BOEHM_CFLAGS)
maptiles_LDFLAGS =
maptiles_LDADD = libsfsim2025.la $(BOEHM_LIBS) -lm

scale50_SOURCES = scale50.c
scale50_CFLAGS = $(BOEHM_CFLAGS)
scale50_LDFLAGS =
scale50_LDADD = libsfsim2025.la $(BOEHM_LIBS) -lm

bluemarble-local: world.200404.3x21600x21600.A1.png world.200404.3x21600x21600.A2.png world.200404.3x21600x21600.B1.png \
	world.200404.3x21600x21600.B2.png world.200404.3x21600x21600.C1.png world.200404.3x21600x21600.C2.png \
	world.200404.3x21600x21600.D1.png world.200404.3x21600x21600.D2.png

world.200404.3x21600x21600.A1.png:
	wget https://neo.sci.gsfc.nasa.gov/archive/bluemarble/bmng/world_500m/world.200404.3x21600x21600.A1.png

world.200404.3x21600x21600.A2.png:
	wget https://neo.sci.gsfc.nasa.gov/archive/bluemarble/bmng/world_500m/world.200404.3x21600x21600.A2.png

world.200404.3x21600x21600.B1.png:
	wget https://neo.sci.gsfc.nasa.gov/archive/bluemarble/bmng/world_500m/world.200404.3x21600x21600.B1.png

world.200404.3x21600x21600.B2.png:
	wget https://neo.sci.gsfc.nasa.gov/archive/bluemarble/bmng/world_500m/world.200404.3x21600x21600.B2.png

world.200404.3x21600x21600.C1.png:
	wget https://neo.sci.gsfc.nasa.gov/archive/bluemarble/bmng/world_500m/world.200404.3x21600x21600.C1.png

world.200404.3x21600x21600.C2.png:
	wget https://neo.sci.gsfc.nasa.gov/archive/bluemarble/bmng/world_500m/world.200404.3x21600x21600.C2.png

world.200404.3x21600x21600.D1.png:
	wget https://neo.sci.gsfc.nasa.gov/archive/bluemarble/bmng/world_500m/world.200404.3x21600x21600.D1.png

world.200404.3x21600x21600.D2.png:
	wget https://neo.sci.gsfc.nasa.gov/archive/bluemarble/bmng/world_500m/world.200404.3x21600x21600.D2.png
