#include <stdbool.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glut.h>
#include "sfsim2025/hull.h"

double a = 0;

hull_t *wheel = 0;

static hull_t *make_wheel(int n) {
  hull_t *result = make_hull();
  for (int i=0; i<n; i++) {
    double angle0 = 2 * M_PI * i / n;
    add_point(result, vector(cos(angle0), sin(angle0), +0.1));
    add_point(result, vector(cos(angle0), sin(angle0), -0.1));
  };
  add_point(result, vector(0, 0, +0.1));
  add_point(result, vector(0, 0, -0.1));
  for (int i=0; i<n; i++) {
    int j = (i + 1) % n;
    add_face(result, face(2 * i, 2 * i + 1, 2 * j + 1));
    add_face(result, face(2 * i, 2 * j + 1, 2 * j));
    add_face(result, face(2 * n, 2 * i, 2 * j));
    add_face(result, face(2 * n + 1, 2 * j + 1, 2 * i + 1));
  };
  return result;
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glLoadIdentity();
  glRotated(a, 0, 0, 1);
  glBegin(GL_TRIANGLES);
  for (int i=0; i<wheel->faces.size; i++) {
    face_t face = get_face(wheel->faces)[i];
    vector_t a = get_vector(wheel->points)[face.a];
    vector_t b = get_vector(wheel->points)[face.b];
    vector_t c = get_vector(wheel->points)[face.c];
    glVertex3d(a.x, a.y, a.z - 3);
    glVertex3d(b.x, b.y, b.z - 3);
    glVertex3d(c.x, c.y, c.z - 3);
  };
  glEnd();
  glFlush();
}

void step(void) {
  a += 1;
}

int main(int argc, char *argv[]) {
  GC_INIT();
  glutInit(&argc, argv);
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_Window *window = SDL_CreateWindow("wheel", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                                        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  SDL_GLContext context = SDL_GL_CreateContext(window);
  glViewport(0, 0, 640, 480);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(65.0, (GLfloat)640/(GLfloat)480, 1.0, 20.0);
  bool quit = false;
  wheel = make_wheel(12);
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
