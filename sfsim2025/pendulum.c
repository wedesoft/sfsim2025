#include <gc.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glut.h>
#include "sfsim2025/joint.h"
#include "sfsim2025/mechanics.h"


state_t *s1;
state_t *s2;

double w = 0.3;
double h = 2.0;
double d = 0.1;


void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
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

void *pendulum_change(double t, double dt, void *s_, void *data_) {
  state_t *s2 = s_;
  body_info_t info1 = body_info(5.9742e+24, inertia_sphere(5.9742e+24, 6370000), vector(0, 0, 0), vector(0, 0, 0));
  body_info_t info2 = body_info(1.0, inertia_cuboid(1.0, w, h, d), vector(0, -9.81, 0), vector(0, 0, 0));
  joint_t jnt = joint(vector(0, 6370002, 0), vector(0, 1, 0));
  large_matrix_t j = ball_in_socket_jacobian(s1, s2, jnt);
  large_vector_t b = ball_in_socket_correction(s1, s2, jnt);
  vector_t p1; vector_t p2; vector_t t1; vector_t t2;
  correcting_impulse(info1, info2, s1, s2, j, b, &p1, &p2, &t1, &t2);
  double dt_div_mass = dt / info2.mass;
  matrix_t inertia = rotate_matrix(s2->orientation, info2.inertia);
  state_t *result =
    state(vector_scale(s2->speed, dt),
          vector_add(vector_scale(p2, 1.0 / info2.mass), vector_scale(info2.force, dt_div_mass)),
          quaternion_product(vector_to_quaternion(vector_scale(s2->rotation, 0.5 * dt)), s2->orientation),
          matrix_vector_dot(inverse(inertia), t2));
  return result;
}

void step() {
  double dt = 0.001;
  for (int i=0; i<10; i++) {
    s2 = runge_kutta(s2, dt, pendulum_change, add_states, scale_state, NULL);
  };
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
  s1 = state(vector(0, -6370000, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0));
  s2 = state(vector(1, 2, 0), vector(0, 0, 0), quaternion_rotation(M_PI / 2, vector(0, 0, 1)), vector(0, 0, 0));
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
