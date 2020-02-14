#include <stdbool.h>
#include <gc.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <SDL.h>
#include <SDL_opengl.h>

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glFlush();
}

int main(int argc, char *argv[]) {
  GC_INIT();
  glutInit(&argc, argv);
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_Window *window = SDL_CreateWindow("grid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480,
                                        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  SDL_GLContext context = SDL_GL_CreateContext(window);
  glewExperimental = GL_TRUE;
  glewInit();
  glViewport(0, 0, 640, 480);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(65.0, (GLfloat)640/(GLfloat)480, 1.0, 20.0);
  bool quit = false;
  while (!quit) {
		SDL_Event e;
    SDL_WaitEvent(&e);
    if (e.type == SDL_QUIT)
      quit = true;
    display();
    SDL_GL_SwapWindow(window);
  };
  SDL_DestroyWindow(window);
  SDL_Quit();
  return EXIT_SUCCESS;
}
