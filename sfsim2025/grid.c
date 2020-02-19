#include <stdbool.h>
#include <gc.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "sfsim2025/projection.h"


const char *vertexSource = "#version 130\n\
in mediump vec3 point;\n\
uniform mat4 projection;\n\
void main()\n\
{\n\
  gl_Position = projection * (vec4(point, 1) - vec4(0, 0, 5, 0));\n\
}";

const char *fragmentSource = "#version 130\n\
out mediump vec3 fragColor;\n\
void main()\n\
{\n\
  fragColor = vec3(1, 1, 1);\n\
}";

const int width = 640;
const int height = 480;

void printStatus(const char *step, GLuint context, GLuint status) {
  GLint result = GL_FALSE;
  glGetShaderiv(context, status, &result);
  if (result == GL_FALSE) {
    char buffer[1024];
    if (status == GL_COMPILE_STATUS)
      glGetShaderInfoLog(context, 1024, NULL, buffer);
    else
      glGetProgramInfoLog(context, 1024, NULL, buffer);
    if (buffer[0])
      fprintf(stderr, "%s: %s\n", step, buffer);
  };
}

void printCompileStatus(const char *step, GLuint context) {
  printStatus(step, context, GL_COMPILE_STATUS);
}

void printLinkStatus(const char *step, GLuint context) {
  printStatus(step, context, GL_LINK_STATUS);
}

GLuint vao;
GLuint vbo;
GLuint idx;
GLuint tex;
GLuint program;

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram(program);
  float *proj = projection(width, height, 0.1, 20.0, 60.0);
  glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, proj);
  glDrawElements(GL_TRIANGLES, 6 * 10 * 10 * 2 * 3, GL_UNSIGNED_INT, (void *)0);
  glFlush();
}

int main(int argc, char *argv[]) {
  GC_INIT();
  glutInit(&argc, argv);
  SDL_Init(SDL_INIT_VIDEO);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_Window *window = SDL_CreateWindow("grid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
                                        SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  SDL_GLContext context = SDL_GL_CreateContext(window);
  glewExperimental = GL_TRUE;
  glewInit();

  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);
  printCompileStatus("Vertex shader", vertexShader);

  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
  glCompileShader(fragmentShader);
  printCompileStatus("Fragment shader", fragmentShader);

  program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);
  printLinkStatus("Shader program", program);

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  GLfloat *vertices = GC_MALLOC_ATOMIC(6 * 11 * 11 * 3 * sizeof(GLfloat));
  {
    GLfloat *p = vertices;
    for (int k=0; k<6; k++) {
      for (int j=0; j<11; j++) {
        for (int i=0; i<11; i++) {
          float x, y, z;
          switch (k) {
            case 0:
              x = -1 + 0.2 * i;
              y = -1 + 0.2 * j;
              z = 1;
              break;
            case 1:
              x = -1 + 0.2 * i;
              y = 1 - 0.2 * j;
              z = -1;
              break;
            case 2:
              x = 1 - 0.2 * i;
              y = 1;
              z = -1 + 0.2 * j;
              break;
            case 3:
              x = 1 - 0.2 * i;
              y = -1;
              z = 1 - 0.2 * j;
              break;
            case 4:
              x = 1;
              y = -1 + 0.2 * i;
              z = -1 + 0.2 * j;
              break;
            case 5:
              x = -1;
              y = -1 + 0.2 * i;
              z = 1 - 0.2 * j;
              break;
          };
          double d = sqrt(x * x + y * y + z * z);
          p[0] = x / d;
          p[1] = y / d;
          p[2] = z / d;
          p += 3;
        };
      };
    };
  };

  int *indices = GC_MALLOC_ATOMIC(6 * 10 * 10 * 2 * 3 * sizeof(int));
  {
    int *p = indices;
    for (int k=0; k<6; k++) {
      for (int j=0; j<10; j++) {
        for (int i=0; i<10; i++) {
          p[0] = k * 121 + j * 11 + i;
          p[1] = k * 121 + j * 11 + i + 1;
          p[2] = k * 121 + (j + 1) * 11 + i;
          p += 3;
          p[0] = k * 121 + (j + 1) * 11 + i;
          p[1] = k * 121 + j * 11 + i + 1;
          p[2] = k * 121 + (j + 1) * 11 + i + 1;
          p += 3;
        };
      };
    };
  };

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, 6 * 11 * 11 * 3 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &idx);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 10 * 10 * 2 * 3 * sizeof(int), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(glGetAttribLocation(program, "point"), 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glViewport(0, 0, width, height);
  bool quit = false;
  while (!quit) {
    SDL_Event e;
    SDL_WaitEvent(&e);
    if (e.type == SDL_QUIT)
      quit = true;
    display();
    SDL_GL_SwapWindow(window);
  };

  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &idx);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glDeleteBuffers(1, &vbo);

  glBindVertexArray(0);
  glDeleteVertexArrays(1, &vao);

  glDetachShader(program, vertexShader);
  glDetachShader(program, fragmentShader);
  glDeleteProgram(program);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  SDL_DestroyWindow(window);

  SDL_Quit();
  return EXIT_SUCCESS;
}
