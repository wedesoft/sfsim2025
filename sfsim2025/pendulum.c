#include <gc.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glut.h>
#include "sfsim2025/mechanics.h"
#include "sfsim2025/world.h"


world_t *world;
world_info_t info;

double w = 0.3;
double h = 2.0;
double d = 0.1;


void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  state_t *s2 = get_pointer(world->states)[1];
  matrix_t r = rotation_matrix(s2->orientation);
  double m[16] = {
    r.m11, r.m21, r.m31, 0,
    r.m12, r.m22, r.m32, 0,
    r.m13, r.m23, r.m33, 0,
    s2->position.x, s2->position.y - 1, s2->position.z - 4, 1
  };
  glLoadMatrixd(m);
  glScalef(w, h, d);
  glutWireCube(1.0);
  glFlush();
}

void step() {
  double dt = 0.01;
  int n = 100;
  for (int i=0; i<n; i++)
    world = runge_kutta(world, dt / n, world_change, add_worlds, scale_world, &info);
}

int main(int argc, char *argv[]) {
  GC_INIT();
  glutInit(&argc, argv);
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_Window *window = SDL_CreateWindow("pendulum", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                                        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  SDL_GLContext context = SDL_GL_CreateContext(window);
  glViewport(0, 0, 640, 480);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(65.0, (GLfloat)640/(GLfloat)480, 1.0, 20.0);
  world = make_world();
  append_pointer(&world->states, state(vector(0, -6370000, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0)));
  append_pointer(&world->states, state(vector(1, 2, 0), vector(0, 0, 0), quaternion_rotation(M_PI / 2, vector(0, 0, 1)), vector(1, 0, 0)));
  info = make_world_info();
  append_body(&info.bodies, body(5.9742e+24, inertia_sphere(5.9742e+24, 6370000)));
  append_body(&info.bodies, body(1.0, inertia_cuboid(1.0, w, h, d)));
  append_forces(&info.forces, forces(vector(0, 9.81, 0), vector(0, 0, 0)));
  append_forces(&info.forces, forces(vector(0, -9.81, 0), vector(0, 0, 0)));
  append_joint(&info.joints, joint(0, 1, vector(0, 6370002, 0), vector(0, 1, 0)));
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
