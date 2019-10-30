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

double w = 0.1;
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

void step() {
  double dt = 0.01;
  body_info_t info1 = body_info(5.9742e+24, inertia_sphere(5.9742e+24, 6370000), vector(0, 0, 0), vector(0, 0, 0));
  body_info_t info2 = body_info(1.0, inertia_cuboid(1.0, w, h, d), vector(0, -1, 0), vector(0, 0, 0));
  double dt_div_mass = dt / info2.mass;
  large_matrix_t j = ball_in_socket(s1, s2, vector(0, 6370002, 0), vector(0, 1, 0));
  large_matrix_t m = joint_mass(info1, info2, s1, s2);
  large_vector_t u = speed_vector(s1, s2);
  large_matrix_t d = large_inverse(large_matrix_dot(large_matrix_dot(j, large_inverse(m)), large_transpose(j)));
  large_vector_t v = large_matrix_vector_dot(j, u);
  large_vector_t l = large_vector_scale(large_matrix_vector_dot(d, v), -1);
  large_vector_t p = large_matrix_vector_dot(large_transpose(j), l);
  vector_t impulse = vector(p.data[6], p.data[7], p.data[8]);
  vector_t rot = vector(p.data[9], p.data[10], p.data[11]);
  matrix_t inertia = rotate_matrix(s2->orientation, info2.inertia);
  s2 = state(vector_add(s2->position, vector_scale(s2->speed, dt)),
             vector_add(s2->speed, vector_add(vector_scale(impulse, 1.0 / info2.mass), vector_scale(info2.force, dt_div_mass))),
             quaternion_add(s2->orientation, quaternion_product(vector_to_quaternion(vector_scale(s2->rotation, 0.5 * dt)), s2->orientation)),
             vector_add(s2->rotation, matrix_vector_dot(inverse(inertia), rot)));
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
