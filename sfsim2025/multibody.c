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
large_matrix_t contact_conditions(list_t contacts, list_t bodies) {
  int rows = 6 * bodies.size;
  int columns = 3 * contacts.size;
  large_matrix_t result = allocate_large_matrix(rows, columns);
  memset(result.data, 0, rows * columns * sizeof(double));
  for (int k=0; k<contacts.size; k++) {
    contact_t contact = get_contact(contacts)[k];
    {
      double *p = &result.data[k * 3 + contact.i * columns * 6];
      *p = -1; p += columns + 1;
      *p = -1; p += columns + 1;
      *p = -1; p += columns - 2;
      vector_t r_i = vector_subtract(contact.point, ((rigid_body_t *)get_pointer(bodies)[contact.i])->center);
      matrix_t m_i = matrix_negative(cross_product_matrix(r_i));
      p[0] = m_i.m11; p[1] = m_i.m12; p[2] = m_i.m13; p += columns;
      p[0] = m_i.m21; p[1] = m_i.m22; p[2] = m_i.m23; p += columns;
      p[0] = m_i.m31; p[1] = m_i.m32; p[2] = m_i.m33; p += columns;
    };
    {
      double *p = &result.data[k * 3 + contact.j * columns * 6];
      *p = 1; p += columns + 1;
      *p = 1; p += columns + 1;
      *p = 1; p += columns - 2;
      vector_t r_j = vector_subtract(contact.point, ((rigid_body_t *)get_pointer(bodies)[contact.j])->center);
      matrix_t m_j = cross_product_matrix(r_j);
      p[0] = m_j.m11; p[1] = m_j.m12; p[2] = m_j.m13; p += columns;
      p[0] = m_j.m21; p[1] = m_j.m22; p[2] = m_j.m23; p += columns;
      p[0] = m_j.m31; p[1] = m_j.m32; p[2] = m_j.m33; p += columns;
    };
  };
  return result;
}

// Create generalized velocity vector.
large_vector_t velocity_vector(list_t states) {
  int n = states.size;
  large_vector_t result = allocate_large_vector(6 * n);
  double *p = result.data;
  for (int i=0; i<n; i++) {
    state_t *s = get_pointer(states)[i];
    *p++ = s->speed.x;
    *p++ = s->speed.y;
    *p++ = s->speed.z;
    *p++ = s->rotation.x;
    *p++ = s->rotation.y;
    *p++ = s->rotation.z;
  };
  return result;
}