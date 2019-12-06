#include <assert.h>
#include "contact.h"
#include "joint.h"


// Compute relative speed of two objects at a given point.
vector_t relative_speed(state_t *state1, state_t *state2, vector_t point) {
  vector_t r1 = vector_subtract(point, state1->position);
  vector_t r2 = vector_subtract(point, state2->position);
  vector_t angular_speed1 = cross_product(state1->rotation, r1);
  vector_t angular_speed2 = cross_product(state2->rotation, r2);
  return vector_subtract(vector_add(state2->speed, angular_speed2), vector_add(state1->speed, angular_speed1));
}

// Construct Jacobian for a contact point of two objects.
// http://image.diku.dk/kenny/download/erleben.05.thesis.pdf
large_matrix_t contact_jacobian(contact_t contact, state_t *state1, state_t *state2) {
#ifdef FRICTION
  large_matrix_t result = allocate_large_matrix(3, 12);
#else
  large_matrix_t result = allocate_large_matrix(1, 12);
#endif
  vector_t u = orthogonal1(contact.normal);
  vector_t v = orthogonal2(contact.normal);
  vector_t r1 = vector_subtract(contact.point, state1->position);
  vector_t r2 = vector_subtract(contact.point, state2->position);
  vector_t rn1 = cross_product(r1, contact.normal);
  vector_t rn2 = cross_product(r2, contact.normal);
  vector_t ru1 = cross_product(r1, u);
  vector_t ru2 = cross_product(r2, u);
  vector_t rv1 = cross_product(r1, v);
  vector_t rv2 = cross_product(r2, v);
  double *p0 = result.data;
  *p0++ = -contact.normal.x; *p0++ = -contact.normal.y; *p0++ = -contact.normal.z; p0+= 9;
#ifdef FRICTION
  *p0++ = u.x; *p0++ = u.y; *p0++ = u.z; p0 += 9;
  *p0++ = v.x; *p0++ = v.y; *p0++ = v.z; p0 += 9;
#endif
  double *p1 = result.data + 3;
  *p1++ = -rn1.x; *p1++ = -rn1.y; *p1++ = -rn1.z; p1 += 9;
#ifdef FRICTION
  *p1++ = ru1.x; *p1++ = ru1.y; *p1++ = ru1.z; p1 += 9;
  *p1++ = rv1.x; *p1++ = rv1.y; *p1++ = rv1.z; p1 += 9;
#endif
  double *p2 = result.data + 6;
  *p2++ = contact.normal.x; *p2++ = contact.normal.y; *p2++ = contact.normal.z; p2+= 9;
#ifdef FRICTION
  *p2++ = -u.x; *p2++ = -u.y; *p2++ = -u.z; p2 += 9;
  *p2++ = -v.x; *p2++ = -v.y; *p2++ = -v.z; p2 += 9;
#endif
  double *p3 = result.data + 9;
  *p3++ = rn2.x; *p3++ = rn2.y; *p3++ = rn2.z; p3 += 9;
#ifdef FRICTION
  *p3++ = -ru2.x; *p3++ = -ru2.y; *p3++ = -ru2.z; p3 += 9;
  *p3++ = -rv2.x; *p3++ = -rv2.y; *p3++ = -rv2.z; p3 += 9;
#endif
  return result;
}

// Compute correction for a contact point of two objects.
// http://image.diku.dk/kenny/download/erleben.05.thesis.pdf
large_vector_t contact_correction(contact_t contact, bool do_restitution) {
#ifdef FRICTION
  large_vector_t result = allocate_large_vector(3);
#else
  large_vector_t result = allocate_large_vector(1);
#endif
  double correction;
  if (do_restitution) {
    // Only spearating collision impulses are allowed.
    correction = contact.normal_speed < 0 ? contact.normal_speed * contact.restitution : 0.0;
  } else {
    // Only separating stabilization is allowed.
    correction = contact.distance < 0 ? contact.distance : 0.0;
  };
  result.data[0] = correction;
#ifdef FRICTION
  result.data[1] = 0;
  result.data[2] = 0;
#endif
  return result;
}

// Compute impulse caused by contact point of two objects.
void contact_impulse(body_t body1, body_t body2, state_t *state1, state_t *state2, contact_t contact,
                     vector_t *impulse1, vector_t *impulse2, vector_t *tau1, vector_t *tau2, bool do_restitution) {
  large_matrix_t j = contact_jacobian(contact, state1, state2);
  large_vector_t b = contact_correction(contact, do_restitution);
  large_vector_t lambda_ = lambda(body1, body2, state1, state2, j, b);
#ifdef FRICTION
  assert(lambda_.rows == 3);
#endif
  // Only separating impulses are allowed.
  if (lambda_.data[0] < 0)
    lambda_.data[0] = 0;
#ifdef FRICTION
  double friction = sqrt(lambda_.data[1] * lambda_.data[1] + lambda_.data[2] * lambda_.data[2]);
  if (friction > lambda_.data[0] * contact.friction) {
    double factor = lambda_.data[0] * contact.friction / friction;
    lambda_.data[1] *= factor;
    lambda_.data[2] *= factor;
  };
#endif
  apply_lambda(j, lambda_, impulse1, impulse2, tau1, tau2);
}
