noinst_PROGRAMS = tumble pendulum chain collide spring gear wheel car icosahedron maptiles scale-image scale-elevation \
									elevationtiles grid

noinst_SCRIPTS = multiresmap.sh multireselevation.sh

lib_LTLIBRARIES = libsfsim2025.la

pkginclude_HEADERS = body.h contact.h coordinate.h edge.h elevation.h face.h force.h friction.h image.h joint.h large_matrix.h \
										 large_vector.h list.h matrix.h mechanics.h plane.h polygon.h quaternion.h hull.h state.h vector.h world.h

libsfsim2025_la_SOURCES = contact.c elevation.c force.c friction.c image.c joint.c large_matrix.c large_vector.c list.c matrix.c \
													mechanics.c plane.c polygon.c quaternion.c hull.c state.c vector.c world.c
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

scale_image_SOURCES = scale_image.c
scale_image_CFLAGS = $(BOEHM_CFLAGS)
scale_image_LDFLAGS =
scale_image_LDADD = libsfsim2025.la $(BOEHM_LIBS) -lm

scale_elevation_SOURCES = scale_elevation.c
scale_elevation_CFLAGS = $(BOEHM_CFLAGS)
scale_elevation_LDFLAGS =
scale_elevation_LDADD = libsfsim2025.la $(BOEHM_LIBS) -lm

elevationtiles_SOURCES = elevationtiles.c
elevationtiles_CFLAGS = $(BOEHM_CFLAGS)
elevationtiles_LDFLAGS =
elevationtiles_LDADD = libsfsim2025.la $(BOEHM_LIBS) -lm

grid_SOURCES = grid.c
grid_CFLAGS = $(GLEW_CFLAGS) $(GL_CFLAGS) $(ATLAS_CFLAGS) $(BOEHM_CFLAGS) $(SDL_CFLAGS)
grid_LDFLAGS =
grid_LDADD = $(GLEW_LIBS) $(GL_LIBS) $(BOEHM_LIBS) $(SDL_LIBS) -lm

world.200404.3x21600x21600.A1.png:
	$(WGET) https://neo.sci.gsfc.nasa.gov/archive/bluemarble/bmng/world_500m/world.200404.3x21600x21600.A1.png

world.200404.3x21600x21600.A2.png:
	$(WGET) https://neo.sci.gsfc.nasa.gov/archive/bluemarble/bmng/world_500m/world.200404.3x21600x21600.A2.png

world.200404.3x21600x21600.B1.png:
	$(WGET) https://neo.sci.gsfc.nasa.gov/archive/bluemarble/bmng/world_500m/world.200404.3x21600x21600.B1.png

world.200404.3x21600x21600.B2.png:
	$(WGET) https://neo.sci.gsfc.nasa.gov/archive/bluemarble/bmng/world_500m/world.200404.3x21600x21600.B2.png

world.200404.3x21600x21600.C1.png:
	$(WGET) https://neo.sci.gsfc.nasa.gov/archive/bluemarble/bmng/world_500m/world.200404.3x21600x21600.C1.png

world.200404.3x21600x21600.C2.png:
	$(WGET) https://neo.sci.gsfc.nasa.gov/archive/bluemarble/bmng/world_500m/world.200404.3x21600x21600.C2.png

world.200404.3x21600x21600.D1.png:
	$(WGET) https://neo.sci.gsfc.nasa.gov/archive/bluemarble/bmng/world_500m/world.200404.3x21600x21600.D1.png

world.200404.3x21600x21600.D2.png:
	$(WGET) https://neo.sci.gsfc.nasa.gov/archive/bluemarble/bmng/world_500m/world.200404.3x21600x21600.D2.png

world/00/00.png: world.200404.3x21600x21600.A1.png
	sh ./multiresmap.sh $< $(basename $@)

world/00/10.png: world.200404.3x21600x21600.A2.png
	sh ./multiresmap.sh $< $(basename $@)

world/00/01.png: world.200404.3x21600x21600.B1.png
	sh ./multiresmap.sh $< $(basename $@)

world/00/11.png: world.200404.3x21600x21600.B2.png
	sh ./multiresmap.sh $< $(basename $@)

world/01/00.png: world.200404.3x21600x21600.C1.png
	sh ./multiresmap.sh $< $(basename $@)

world/01/10.png: world.200404.3x21600x21600.C2.png
	sh ./multiresmap.sh $< $(basename $@)

world/01/01.png: world.200404.3x21600x21600.D1.png
	sh ./multiresmap.sh $< $(basename $@)

world/01/11.png: world.200404.3x21600x21600.D2.png
	sh ./multiresmap.sh $< $(basename $@)

bluemarble-local: world/00/00.png world/00/01.png world/00/10.png world/00/11.png \
	world/01/00.png world/01/01.png world/01/10.png world/01/11.png

all10: all10g.tgz
	tar -xvf $<
	touch $@

elevation.A1.raw: all10
	cat all10/a10g all10/e10g > $@

elevation.B1.raw: all10
	cat all10/b10g all10/f10g > $@

elevation.C1.raw: all10
	cat all10/c10g all10/g10g > $@

elevation.D1.raw: all10
	cat all10/d10g all10/h10g > $@

elevation.A2.raw: all10
	cat all10/i10g all10/m10g > $@

elevation.B2.raw: all10
	cat all10/j10g all10/n10g > $@

elevation.C2.raw: all10
	cat all10/k10g all10/o10g > $@

elevation.D2.raw: all10
	cat all10/l10g all10/p10g > $@

elevation/00/00.raw: elevation.A1.raw
	sh ./multireselevation.sh $< $(basename $@)

elevation/00/10.raw: elevation.A2.raw
	sh ./multireselevation.sh $< $(basename $@)

elevation/00/01.raw: elevation.B1.raw
	sh ./multireselevation.sh $< $(basename $@)

elevation/00/11.raw: elevation.B2.raw
	sh ./multireselevation.sh $< $(basename $@)

elevation/01/00.raw: elevation.C1.raw
	sh ./multireselevation.sh $< $(basename $@)

elevation/01/10.raw: elevation.C2.raw
	sh ./multireselevation.sh $< $(basename $@)

elevation/01/01.raw: elevation.D1.raw
	sh ./multireselevation.sh $< $(basename $@)

elevation/01/11.raw: elevation.D2.raw
	sh ./multireselevation.sh $< $(basename $@)

heightfield-local: elevation/00/00.raw elevation/00/01.raw elevation/00/10.raw elevation/00/11.raw \
	elevation/01/00.raw elevation/01/01.raw elevation/01/10.raw elevation/01/11.raw

all10g.tgz:
	$(WGET) https://www.ngdc.noaa.gov/mgg/topo/DATATILES/elev/all10g.tgz
