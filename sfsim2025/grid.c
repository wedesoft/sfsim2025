#include <assert.h>
#include <time.h>
#include <stdbool.h>
#include <gc.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include "sfsim2025/image.h"
#include "sfsim2025/elevation.h"
#include "sfsim2025/projection.h"
#include "sfsim2025/map.h"


const char *vertexSource = "#version 130\n\
in mediump vec3 point;\n\
in mediump vec2 texcoord;\n\
uniform mat4 projection;\n\
uniform mat4 rotation;\n\
out mediump vec2 UV;\n\
void main()\n\
{\n\
  gl_Position = projection * (rotation * vec4(point, 1) - vec4(0, 0, 6378000 * 4, 0));\n\
  UV = texcoord;\n\
}";

const char *fragmentSource = "#version 130\n\
in mediump vec2 UV;\n\
out mediump vec3 fragColor;\n\
uniform sampler2D tex;\n\
uniform sampler2D water;\n\
void main()\n\
{\n\
  float wat = texture(water, UV).y;\n\
  fragColor = texture(tex, UV).rgb * (1 - wat) + wat * vec3(0.2, 0.2, 0.4);\n\
}";

const int width = 1024;
const int height = 768;

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

#define L 0
#define N (1 << L)

GLuint vao[6 * N * N];
GLuint vbo[6 * N * N];
GLuint idx[6 * N * N];
GLuint tex[6 * N * N];
GLuint wat[6 * N * N];
GLuint program;
double angle = 0;
struct timespec t0;

void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  float *rot = GC_MALLOC_ATOMIC(16 * sizeof(float));
  rot[0] = cos(angle); rot[4] = 0; rot[ 8] = -sin(angle); rot[12] = 0;
  rot[1] =          0; rot[5] = 1; rot[ 9] =           0; rot[13] = 0;
  rot[2] = sin(angle); rot[6] = 0; rot[10] =  cos(angle); rot[14] = 0;
  rot[3] =          0; rot[7] = 0; rot[11] =           0; rot[15] = 1;
  float *proj = projection(width, height, 1000, 6378000.0 * 4, 45.0);
  for (int k=0; k<6; k++) {
    for (int b=0; b<N; b++) {
      for (int a=0; a<N; a++) {
        glBindVertexArray(vao[k * N * N + b * N + a]);
        glUseProgram(program);
        glUniformMatrix4fv(glGetUniformLocation(program, "rotation"), 1, GL_FALSE, rot);
        glUniformMatrix4fv(glGetUniformLocation(program, "projection"), 1, GL_FALSE, proj);
        glActiveTexture(GL_TEXTURE0 + 0);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, tex[k * N * N + b * N + a]);
        glActiveTexture(GL_TEXTURE0 + 1);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, wat[k * N * N + b * N + a]);
        glDrawElements(GL_TRIANGLES, 255 * 255 * 2 * 3, GL_UNSIGNED_INT, (void *)0);
      };
    };
  };
  glFlush();
}

void step(void) {
  struct timespec t1;
  clock_gettime(CLOCK_REALTIME, &t1);
  double dt = fmin(t1.tv_sec - t0.tv_sec + (t1.tv_nsec - t0.tv_nsec) * 1e-9, 0.1);
  angle = fmod(angle + 0.1 * dt, 2 * M_PI);
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

  for (int k=0; k<6; k++) {
    for (int b=0; b<N; b++) {
      for (int a=0; a<N; a++) {
        glGenVertexArrays(1, &vao[k * N * N + b * N + a]);
        glBindVertexArray(vao[k * N * N + b * N + a]);

        water_t water = read_water(cubepath("globe", k, L, b, a, ".bin"));
        image_t img = read_image(cubepath("globe", k, L, b, a, ".png"));
        assert(img.data);
        vertex_tile_t vertices = read_vertex_tile(cubepath("globe", k, L, b, a, ".raw"));
        int *indices = cube_indices(256);

        glGenBuffers(1, &vbo[k * N * N + b * N + a]);
        glBindBuffer(GL_ARRAY_BUFFER, vbo[k * N * N + b * N + a]);
        glBufferData(GL_ARRAY_BUFFER, 256 * 256 * 5 * sizeof(GLfloat), vertices.data, GL_STATIC_DRAW);

        glGenBuffers(1, &idx[k * N * N + b * N + a]);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, idx[k * N * N + b * N + a]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 255 * 255 * 2 * 3 * sizeof(int), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(glGetAttribLocation(program, "point"), 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
        glVertexAttribPointer(glGetAttribLocation(program, "texcoord"), 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glActiveTexture(GL_TEXTURE0 + 0);
        glGenTextures(1, &tex[k * N * N + b * N + a]);
        glBindTexture(GL_TEXTURE_2D, tex[k * N * N + b * N + a]);
        glUseProgram(program);
        glUniform1i(glGetUniformLocation(program, "tex"), 0);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img.width, img.height, 0, GL_RGB, GL_UNSIGNED_BYTE, img.data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glActiveTexture(GL_TEXTURE0 + 1);
        glGenTextures(1, &wat[k * N * N + b * N + a]);
        glBindTexture(GL_TEXTURE_2D, wat[k * N * N + b * N + a]);
        glUseProgram(program);
        glUniform1i(glGetUniformLocation(program, "water"), 1);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, water.width, water.height, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, water.data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
      };
    };
  };

  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);

  glViewport(0, 0, width, height);
  clock_gettime(CLOCK_REALTIME, &t0);
  bool quit = false;
  while (!quit) {
    SDL_Event e;
    while (SDL_PollEvent(&e) != 0) {
      if (e.type == SDL_QUIT)
        quit = true;
    };
    display();
    step();
    SDL_GL_SwapWindow(window);
  };

  for (int k=0; k<6 * N * N; k++) {
    glBindVertexArray(vao[k]);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &idx[k]);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &vbo[k]);

    glBindVertexArray(0);
    glDeleteVertexArrays(1, &vao[k]);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &tex[k]);

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &wat[k]);
  };

  glDetachShader(program, vertexShader);
  glDetachShader(program, fragmentShader);
  glDeleteProgram(program);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  SDL_DestroyWindow(window);

  SDL_Quit();
  return EXIT_SUCCESS;
}
