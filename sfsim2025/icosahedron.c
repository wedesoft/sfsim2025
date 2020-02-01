#include <stdbool.h>
#include <time.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glut.h>
#include "sfsim2025/mechanics.h"
#include "sfsim2025/hull.h"
#include "sfsim2025/world.h"

world_t *world = 0;
world_info_t info;
struct timespec t0;

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  hull_t *icosahedron = get_pointer(info.rigid_bodies)[1];
  state_t *s = get_pointer(world->states)[1];
  matrix_t r = rotation_matrix(s->orientation);
  double m[16] = {
    r.m11, r.m21, r.m31, 0,
    r.m12, r.m22, r.m32, 0,
    r.m13, r.m23, r.m33, 0,
    fmod(s->position.x + 8, 16) - 8, s->position.y, -10, 1
  };
  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixd(m);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBegin(GL_TRIANGLES);
  for (int i=0; i<icosahedron->faces.size; i++) {
    face_t face = get_face(icosahedron->faces)[i];
    vector_t a = get_vector(icosahedron->points)[face.a];
    vector_t b = get_vector(icosahedron->points)[face.b];
    vector_t c = get_vector(icosahedron->points)[face.c];
    glVertex3d(a.x, a.y, a.z);
    glVertex3d(b.x, b.y, b.z);
    glVertex3d(c.x, c.y, c.z);
  };
  glEnd();
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
  SDL_Window *window = SDL_CreateWindow("icosahedron", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                                        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  SDL_GLContext context = SDL_GL_CreateContext(window);
  glViewport(0, 0, 640, 480);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(65.0, (GLfloat)640/(GLfloat)480, 1.0, 20.0);
  world = make_world();
  info = make_world_info();
  info.restitution = 0.2;
  append_pointer(&world->states, state(vector(0, -6370000, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0)));
  append_body(&info.bodies, body(5.9742e+24, inertia_sphere(5.9742e+24, 6370000)));
  append_pointer(&info.rigid_bodies, make_cube(2 * 6370000, 2 * 6370000, 2 * 6370000));
  append_pointer(&world->states, state(vector(0, 4, 0), vector(20, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0.2)));
  append_body(&info.bodies, body(1, inertia_sphere(1, 1)));
  append_pointer(&info.rigid_bodies, make_icosahedron(1));
  append_force(&info.forces, gravitation(0, 1));
  append_contact_candidate(&info.contact_candidates, contact_candidate(0, 1, uniform_friction(0.5)));
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
  SDL_GL_DeleteContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}
