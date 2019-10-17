#include "multibody.h"
#include "matrix43.h"


// Create adapter matrix for position and orientation changes of n bodies.
// https://people.mpi-inf.mpg.de/~schoemer/publications/VRST98.pdf
large_matrix_t state_adapter(int n, quaternion_t q[]) {
  int rows = 7 * n;
  int columns = 6 * n;
  large_matrix_t result = allocate_large_matrix(rows, columns);
  memset(result.data, 0, rows * columns * sizeof(double));
  double *p = result.data;
  for (int l=0; l<n; l++) {
    p[0] = 1; p += columns;
    p[1] = 1; p += columns;
    p[2] = 1; p += columns;
    p += 3;
    matrix43_t o = quaternion_matrix(q[l]);
    p[0] =  o.m11; p[1] = o.m12; p[2] = o.m13; p += columns;
    p[0] =  o.m21; p[1] = o.m22; p[2] = o.m23; p += columns;
    p[0] =  o.m31; p[1] = o.m32; p[2] = o.m33; p += columns;
    p[0] =  o.m41; p[1] = o.m42; p[2] = o.m43; p += columns;
    p += 3;
  };
  return result;
}

// Create generalized mass matrix for n rigid bodies.
// https://people.mpi-inf.mpg.de/~schoemer/publications/VRST98.pdf
large_matrix_t generalized_mass(int n, double mass[], matrix_t inertia[]) {
  int rows = 6 * n;
  int columns = 6 * n;
  large_matrix_t result = allocate_large_matrix(rows, columns);
  memset(result.data, 0, rows * columns * sizeof(double));
  double *p = result.data;
  for (int l=0; l<n; l++) {
    p[0] = mass[l]; p += columns;
    p[1] = mass[l]; p += columns;
    p[2] = mass[l]; p += columns;
    p += 3;
    matrix_t *i = &inertia[l];
    p[0] =  i->m11; p[1] = i->m12; p[2] = i->m13; p += columns;
    p[0] =  i->m21; p[1] = i->m22; p[2] = i->m23; p += columns;
    p[0] =  i->m31; p[1] = i->m32; p[2] = i->m33; p += columns;
    p += 3;
  };
  return result;
}

// Create matrix of contact normals for k contacts.
// https://people.mpi-inf.mpg.de/~schoemer/publications/VRST98.pdf
large_matrix_t contact_normals(int k, vector_t normals[]) {
  int rows = 3 * k;
  int columns = k;
  large_matrix_t result = allocate_large_matrix(rows, columns);
  memset(result.data, 0, rows * columns * sizeof(double));
  double *p = result.data;
  for (int i=0; i<k; i++) {
    vector_t *normal = &normals[i];
    *p = normal->x; p += columns;
    *p = normal->y; p += columns;
    *p = normal->z; p += columns;
    p++;
  };
  return result;
}

// Create matrix of contact conditions.
// https://people.mpi-inf.mpg.de/~schoemer/publications/VRST98.pdf
large_matrix_t contact_conditions(list_t *contacts, list_t *bodies) {
  int k = contacts->size;
  int n = bodies->size;
  int rows = 6 * n;
  int columns = 3 * k;
  large_matrix_t result = allocate_large_matrix(rows, columns);
  return result;
}
