#include <gc.h>
#include <stdlib.h>
#include <time.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glut.h>
#include "sfsim2025/mechanics.h"
#include "sfsim2025/rigid_body.h"
#include "sfsim2025/world.h"

world_t *world;
world_info_t info;
struct timespec t0;

double w = 2.0;
double h = 0.4;
double d = 1.0;
int n = 3;

static void add_cube_faces(rigid_body_t *body) {
  add_face(body, face(0, 1, 3));
  add_face(body, face(0, 3, 2));
  add_face(body, face(5, 4, 7));
  add_face(body, face(5, 6, 7));
  add_face(body, face(4, 0, 2));
  add_face(body, face(4, 2, 6));
  add_face(body, face(1, 5, 7));
  add_face(body, face(1, 7, 3));
  add_face(body, face(0, 5, 1));
  add_face(body, face(0, 4, 5));
  add_face(body, face(2, 3, 7));
  add_face(body, face(2, 7, 6));
}

static rigid_body_t *make_cube(double w2, double h2, double d2) {
  rigid_body_t *result = make_rigid_body();
  add_point(result, vector(-w2, -h2, -d2));
  add_point(result, vector(+w2, -h2, -d2));
  add_point(result, vector(-w2, -h2, +d2));
  add_point(result, vector(+w2, -h2, +d2));
  add_point(result, vector(-w2, +h2, -d2));
  add_point(result, vector(+w2, +h2, -d2));
  add_point(result, vector(-w2, +h2, +d2));
  add_point(result, vector(+w2, +h2, +d2));
  add_cube_faces(result);
  return result;
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  for (int i=0; i<n; i++) {
    state_t *s = get_pointer(world->states)[i+1];
    matrix_t r = rotation_matrix(s->orientation);
    double m[16] = {
      r.m11, r.m21, r.m31, 0,
      r.m12, r.m22, r.m32, 0,
      r.m13, r.m23, r.m33, 0,
      s->position.x, s->position.y - 1, s->position.z - 5, 1
    };
    glLoadMatrixd(m);
    glScalef(w, h, d);
    glutWireCube(1.0);
  };
  glFlush();
}

void step() {
  struct timespec t1;
  clock_gettime(CLOCK_REALTIME, &t1);
  double dt = fmin(t1.tv_sec - t0.tv_sec + (t1.tv_nsec - t0.tv_nsec) * 1e-9, 0.025);
  int iterations = 5;
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
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1);
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
  append_pointer(&info.rigid_bodies, make_cube(6370000, 6370000, 6370000));
  append_forces(&info.forces, forces(vector(0, 0, 0), vector(0, 0, 0)));
  for (int i=0; i<n; i++) {
    append_pointer(&world->states, state(vector(0, 2 + 2 * i, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0.4, 0.0, 0.8)));
    append_body(&info.bodies, body(1.0, inertia_cuboid(1.0, w, h, d)));
    append_pointer(&info.rigid_bodies, make_cube(w / 2, h / 2, d / 2));
    append_forces(&info.forces, forces(vector(0, -9.81, 0), vector(0, 0, 0)));
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
