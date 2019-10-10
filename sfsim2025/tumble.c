#define SDL_MAIN_HANDLED
#include <gc.h>
#include <time.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glut.h>
#include "sfsim2025/mechanics.h"


static state_t *s = 0;
struct timespec t0;
double w = 2.0;
double h = 0.4;
double d = 1.0;

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  matrix_t r = rotation_matrix(s->orientation);
  double m[16] = {
    r.m11, r.m21, r.m31, 0,
    r.m12, r.m22, r.m32, 0,
    r.m13, r.m23, r.m33, 0,
    s->position.x, s->position.y, s->position.z, 1
  };
  glLoadMatrixd(m);
  glScalef(w, h, d);
  glutWireCube(1.0);
  glFlush();
}

void step() {
  struct timespec t1;
  clock_gettime(CLOCK_REALTIME, &t1);
  double elapsed = t1.tv_sec - t0.tv_sec + (t1.tv_nsec - t0.tv_nsec) * 1e-9;
  body_info_t data = {.mass = 1, .inertia = inertia_cuboid(1, w, h, d), .force = vector(0, 0, 0), .torque = vector(0, 0, 0)};
  s = runge_kutta(s, elapsed, state_change, add_states, scale_state, &data);
  t0 = t1;
}

// https://lazyfoo.net/tutorials/SDL/
int main(int argc, char **argv) {
  glutInit(&argc, argv);
  SDL_Init(SDL_INIT_VIDEO);
  GC_INIT();
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_Window *window = SDL_CreateWindow("tumble", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                                        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  SDL_GLContext context = SDL_GL_CreateContext(window);
  glViewport(0, 0, 640, 480);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(65.0, (GLfloat)1024/(GLfloat)768, 1.0, 20.0);
  s = state(vector(0, 0, -4), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0.02, 0.02, 0.8));
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
