#include <gc.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glut.h>
#include "sfsim2025/mechanics.h"
#include "sfsim2025/hull.h"
#include "sfsim2025/world.h"

world_t *world;
world_info_t info;
struct timespec t0;

double w = 2.0;
double h = 0.4;
double d = 1.0;
int n = 3;

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  for (int i=0; i<n; i++) {
    state_t *s = get_pointer(world->states)[i+1];
    matrix_t r = rotation_matrix(s->orientation);
    double m[16] = {
      r.m11, r.m21, r.m31, 0,
      r.m12, r.m22, r.m32, 0,
      r.m13, r.m23, r.m33, 0,
      s->position.x, s->position.y, s->position.z - 5, 1
    };
    glLoadMatrixd(m);
    glScalef(w, h, d);
    glutWireCube(1.0);
  };
  glFlush();
}

void step(void) {
  struct timespec t1;
  clock_gettime(CLOCK_REALTIME, &t1);
  double dt = fmin(t1.tv_sec - t0.tv_sec + (t1.tv_nsec - t0.tv_nsec) * 1e-9, 0.25);
  int iterations = 2;
  for (int i=0; i<iterations; i++) {
    world = euler(world, 0, world_change, add_worlds, scale_world, &info);
    world = runge_kutta(world, dt / iterations, world_change, add_worlds, scale_world, &info);
  };
  t0 = t1;
}

int main(int argc, char *argv[]) {
  GC_INIT();
  glutInit(&argc, argv);
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_Window *window = SDL_CreateWindow("collide", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                                        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  SDL_GLContext context = SDL_GL_CreateContext(window);
  glViewport(0, 0, 640, 480);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(65.0, (GLfloat)640/(GLfloat)480, 1.0, 20.0);
  world = make_world();
  append_pointer(&world->states, state(vector(0, -6370000, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0)));
  info = make_world_info();
  append_body(&info.bodies, body(5.9742e+24, inertia_sphere(5.9742e+24, 6370000)));
  append_pointer(&info.rigid_bodies, make_cube(2 * 6370000, 2 * 6370000, 2 * 6370000));
  for (int i=0; i<n; i++) {
    append_pointer(&world->states, state(vector(0, 3 + 0.5 * h + 2 * h * i, 0), vector(0, 0, 0),
                   quaternion(1, 0, 0, 0), vector(1, 0, 2)));
    append_body(&info.bodies, body(1.0, inertia_cuboid(1.0, w, h, d)));
    append_pointer(&info.rigid_bodies, make_cube(w, h, d));
    append_force(&info.forces, gravitation(0, i + 1));
    append_contact_candidate(&info.contact_candidates, contact_candidate(0, i + 1));
    for (int j=0; j<i; j++)
      append_contact_candidate(&info.contact_candidates, contact_candidate(j + 1, i + 1));
  };
  clock_gettime(CLOCK_REALTIME, &t0);
  bool quit = false;
  while (!quit) {
		SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT)
        quit = true;
    };
    step();
    display();
    SDL_GL_SwapWindow(window);
  };
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}
