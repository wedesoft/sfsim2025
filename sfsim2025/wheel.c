#include <stdbool.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <GL/glut.h>

void display() {
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glBegin(GL_TRIANGLES);
  const int n = 12;
  for (int i=0; i<n; i++) {
    double angle0 = 2 * M_PI * i / n;
    double angle1 = angle0 + 2 * M_PI / n;
    glVertex3d(cos(angle0), sin(angle0), -3.2);
    glVertex3d(cos(angle1), sin(angle1), -3.2);
    glVertex3d(cos(angle0), sin(angle0), -3.0);
    glVertex3d(cos(angle0), sin(angle0), -3.0);
    glVertex3d(cos(angle1), sin(angle1), -3.0);
    glVertex3d(cos(angle1), sin(angle1), -3.2);
  };
  glEnd();
  glFlush();
}

void step(void) {
}

int main(int argc, char *argv[]) {
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
