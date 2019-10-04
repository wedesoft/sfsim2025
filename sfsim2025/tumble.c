#include <tgmath.h>
#include <GL/glut.h>
#include <gc.h>
#include <time.h>
#include "sfsim2025/mechanics.h"


static state_t *s = 0;

void wireBox(GLdouble width, GLdouble height, GLdouble depth) {
  glPushMatrix();
  glScalef(width, height, depth);
  glutWireCube(1.0);
  glPopMatrix();
}

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
  wireBox(w, h, d);
  glFlush();
}

void reshape(GLint w, GLint h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(65.0, (GLfloat)w/(GLfloat)h, 1.0, 20.0);
}

struct timespec t0;

void idle() {
  struct timespec t1;
  clock_gettime(CLOCK_REALTIME, &t1);
  double elapsed = t1.tv_sec - t0.tv_sec + (t1.tv_nsec - t0.tv_nsec) * 1e-9;
  body_info_t data = {
    .mass = 1,
    .inertia = inertia_cuboid(1, w, h, d),
    .force = vector(0, 0, 0),
    .torque = vector(0, 0, 0)
  };
  s = runge_kutta(s, elapsed, state_change, add_states, scale_state, &data);
  glutPostRedisplay();
  t0 = t1;
}

void init() {
  glShadeModel(GL_FLAT);
}

int main(int argc, char** argv) {
  GC_INIT();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
  glutInitWindowPosition(80, 80);
  glutInitWindowSize(800, 600);
  glutCreateWindow("tumble");
  s = state(vector(0, 0, -4), vector(0, 0, 0), quaternion(1, 0, 0, 0), vector(0.02, 0.02, 0.4));
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutIdleFunc(idle);
  init();
  clock_gettime(CLOCK_REALTIME, &t0);
  glutMainLoop();
}
