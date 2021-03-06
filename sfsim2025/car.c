#include <time.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glut.h>
#include "sfsim2025/mechanics.h"
#include "sfsim2025/world.h"

world_t *world = 0;
world_info_t info;
struct timespec t0;

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  for (int k=1; k<world->states.size; k++) {
    state_t *s = get_pointer(world->states)[k];
    matrix_t r = rotation_matrix(s->orientation);
    double m[16] = {
      r.m11, r.m21, r.m31, 0,
      r.m12, r.m22, r.m32, 0,
      r.m13, r.m23, r.m33, 0,
      fmod(s->position.x + 10, 20) - 10, s->position.y, s->position.z - 10, 1
    };
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd(m);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glBegin(GL_TRIANGLES);
    hull_t *hull = get_pointer(info.rigid_bodies)[k];
    for (int i=0; i<hull->faces.size; i++) {
      face_t face = get_face(hull->faces)[i];
      vector_t a = get_vector(hull->points)[face.a];
      vector_t b = get_vector(hull->points)[face.b];
      vector_t c = get_vector(hull->points)[face.c];
      glVertex3d(a.x, a.y, a.z);
      glVertex3d(b.x, b.y, b.z);
      glVertex3d(c.x, c.y, c.z);
    };
    glEnd();
  };
  glFlush();
}

void step(void) {
  struct timespec t1;
  clock_gettime(CLOCK_REALTIME, &t1);
  double dt = fmin(t1.tv_sec - t0.tv_sec + (t1.tv_nsec - t0.tv_nsec) * 1e-9, 0.1);
  int iterations = 1;
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
  SDL_Window *window = SDL_CreateWindow("car", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                                        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  SDL_GLContext context = SDL_GL_CreateContext(window);
  glViewport(0, 0, 640, 480);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(65.0, (GLfloat)640/(GLfloat)480, 1.0, 20.0);
  world = make_world();
  info = make_world_info();
  info.iterations = 10;
  double v0 = 20;
  // 0. cube-shaped planet
  append_pointer(&world->states, state(vector(0, -6370000, 0), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0)));
  append_body(&info.bodies, body(5.9742e+24, inertia_sphere(5.9742e+24, 6370000)));
  append_pointer(&info.rigid_bodies, make_cube(2 * 6370000, 2 * 6370000, 2 * 6370000));
  // 1. main body
  append_pointer(&world->states, state(vector(0, 2, 0), vector(v0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0)));
  append_body(&info.bodies, body(10, inertia_cuboid(10, 4, 1, 2)));
  append_pointer(&info.rigid_bodies, make_cube(4, 1, 2));
  append_force(&info.forces, gravitation(0, 1));
  append_contact_candidate(&info.contact_candidates, contact_candidate(0, 1, uniform_friction(0.5)));
  // 2. front wheel
  append_pointer(&world->states, state(vector(2, 1, 0), vector(v0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0)));
  append_body(&info.bodies, body(0.5, inertia_cuboid(0.5, 0.8, 0.8, 0.8)));
  append_pointer(&info.rigid_bodies, make_wheel(0.4, 0.2, 12));
  append_force(&info.forces, gravitation(0, 2));
  append_joint(&info.joints, slider(1, 2, vector(2, -0.5, 0), vector(0, 0, 0),
                                    quaternion_rotation(M_PI / 2, vector(0, 0, 1)), quaternion_rotation(M_PI / 2, vector(0, 0, 1))));
  append_force(&info.forces, spring_damper(1, 2, vector(2, 2, 0), vector(0, 0, 0), 3, 300, 25));
  append_contact_candidate(&info.contact_candidates, contact_candidate(0, 2, uniform_friction(0.5)));
  // 3. left back wheel
  append_pointer(&world->states, state(vector(-2, 1, -1), vector(v0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0)));
  append_body(&info.bodies, body(0.5, inertia_cuboid(0.5, 0.8, 0.8, 0.8)));
  append_pointer(&info.rigid_bodies, make_wheel(0.4, 0.2, 12));
  append_force(&info.forces, gravitation(0, 3));
  append_joint(&info.joints, slider(1, 3, vector(-2, -0.5, -1), vector(0, 0, 0),
                                    quaternion_rotation(M_PI / 2, vector(0, 0, 1)), quaternion_rotation(M_PI / 2, vector(0, 0, 1))));
  append_force(&info.forces, spring_damper(1, 3, vector(-2, 2, -1), vector(0, 0, 0), 3, 150, 12));
  append_contact_candidate(&info.contact_candidates, contact_candidate(0, 3, anisotropic_friction(0.5, 1)));
  // 4. right back wheel
  append_pointer(&world->states, state(vector(-2, 1, +1), vector(v0, 0, 0), quaternion(1, 0, 0, 0), vector(0, 0, 0)));
  append_body(&info.bodies, body(0.5, inertia_cuboid(0.5, 0.8, 0.8, 0.8)));
  append_pointer(&info.rigid_bodies, make_wheel(0.4, 0.2, 12));
  append_force(&info.forces, gravitation(0, 4));
  append_joint(&info.joints, slider(1, 4, vector(-2, -0.5, +1), vector(0, 0, 0),
                                    quaternion_rotation(M_PI / 2, vector(0, 0, 1)), quaternion_rotation(M_PI / 2, vector(0, 0, 1))));
  append_force(&info.forces, spring_damper(1, 4, vector(-2, 2, +1), vector(0, 0, 0), 3, 150, 12));
  append_contact_candidate(&info.contact_candidates, contact_candidate(0, 4, anisotropic_friction(0.5, 1)));
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
