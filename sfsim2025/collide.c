#include <gc.h>
#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glut.h>
#include "sfsim2025/mechanics.h"
#include "sfsim2025/rigid_body.h"
#include "sfsim2025/world.h"

world_t *world;
world_info_t info;

double w = 2.0;
double h = 0.4;
double d = 1.0;

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

static rigid_body_t *make_cube(double x, double y, double z, double w2, double h2, double d2) {
  rigid_body_t *result = make_rigid_body();
  add_point(result, vector(-w2 + x, -h2 + y, -d2 + z));
  add_point(result, vector(+w2 + x, -h2 + y, -d2 + z));
  add_point(result, vector(-w2 + x, -h2 + y, +d2 + z));
  add_point(result, vector(+w2 + x, -h2 + y, +d2 + z));
  add_point(result, vector(-w2 + x, +h2 + y, -d2 + z));
  add_point(result, vector(+w2 + x, +h2 + y, -d2 + z));
  add_point(result, vector(-w2 + x, +h2 + y, +d2 + z));
  add_point(result, vector(+w2 + x, +h2 + y, +d2 + z));
  add_cube_faces(result);
  return result;
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  state_t *s = get_pointer(world->states)[1];
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
  glFlush();
}

void *world_change_prototype(double time, double dt, void *world_, void *data_) {
  world_t *world = world_;
  world_t *result = make_world();
  world_info_t *data = data_;
  vector_t p[world->states.size]; memset(p, 0, sizeof(p));
  vector_t t[world->states.size]; memset(t, 0, sizeof(t));
  state_t *s1 = get_pointer(world->states)[0];
  state_t *s2 = get_pointer(world->states)[1];
  rigid_body_t *b1 = make_cube(0, -6370000, 0, 6370000, 6370000, 6370000);
  rigid_body_t *b2 = transform_body(make_cube(0, 0, 0, w / 2, h / 2, d / 2), s2->orientation, s2->position);
  body_t body1 = get_body(data->bodies)[0];
  body_t body2 = get_body(data->bodies)[1];
  forces_t forces1 = get_forces(data->forces)[0];
  forces_t forces2 = get_forces(data->forces)[1];
  list_t contacts_ = contacts(0, 1, b1, b2);
  vector_t p1[contacts_.size]; memset(p1, 0, sizeof(p1));
  vector_t p2[contacts_.size]; memset(p2, 0, sizeof(p2));
  vector_t t1[contacts_.size]; memset(t1, 0, sizeof(t1));
  vector_t t2[contacts_.size]; memset(t2, 0, sizeof(t2));
  for (int k=0; k<10; k++) {
    for (int l=0; l<contacts_.size; l++) {
      contact_t c = get_contact(contacts_)[l];
      int i = c.i;
      int j = c.j;
      p[i] = vector_subtract(p[i], p1[l]);
      p[j] = vector_subtract(p[j], p2[l]);
      t[i] = vector_subtract(t[i], t1[l]);
      t[j] = vector_subtract(t[j], t2[l]);
      state_t *prediction1 = predict(s1, body1, forces1, p[i], t[i], dt);
      state_t *prediction2 = predict(s2, body2, forces2, p[j], t[j], dt);
      contact_impulse(body1, body2, s1, s2, c, &p1[l], &p2[l], &t1[l], &t2[l]);
      p[i] = vector_add(p[i], p1[l]);
      p[j] = vector_add(p[j], p2[l]);
      t[i] = vector_add(t[i], t1[l]);
      t[j] = vector_add(t[j], t2[l]);
    };
  };
  for (int i=0; i<world->states.size; i++) {
    state_t *s = get_pointer(world->states)[i];
    body_t b = get_body(data->bodies)[i];
    forces_t f = get_forces(data->forces)[i];
    state_t *change = state_change(s, b, f, p[i], t[i], dt);
    append_pointer(&result->states, change);
  };
  return result;
}

void step() {
  double dt = 0.01;
  int iterations = 3;
  for (int i=0; i<iterations; i++)
    world = runge_kutta(world, dt / iterations, world_change_prototype, add_worlds, scale_world, &info);
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
  append_pointer(&world->states, state(vector(0, 2, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0.3)));
  info = make_world_info();
  info.iterations = 4;
  append_body(&info.bodies, body(5.9742e+24, inertia_sphere(5.9742e+24, 6370000)));
  append_body(&info.bodies, body(1.0, inertia_cuboid(1.0, w, h, d)));
  append_forces(&info.forces, forces(vector(0, 0, 0), vector(0, 0, 0)));
  append_forces(&info.forces, forces(vector(0, -9.81, 0), vector(0, 0, 0)));
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
