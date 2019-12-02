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
  large_matrix_t result = allocate_large_matrix(3, 12);
  vector_t o1 = orthogonal1(contact.normal);
  vector_t o2 = orthogonal2(contact.normal);
  vector_t r1 = vector_subtract(contact.point, state1->position);
  vector_t r2 = vector_subtract(contact.point, state2->position);
  vector_t rn1 = cross_product(r1, contact.normal);
  vector_t rn2 = cross_product(r2, contact.normal);
  vector_t rs1 = cross_product(r1, o1);
  vector_t rs2 = cross_product(r2, o1);
  vector_t rt1 = cross_product(r1, o2);
  vector_t rt2 = cross_product(r2, o2);
  result.data[ 0] = -contact.normal.x;
  result.data[ 1] = -contact.normal.y;
  result.data[ 2] = -contact.normal.z;
  result.data[ 3] = -rn1.x;
  result.data[ 4] = -rn1.y;
  result.data[ 5] = -rn1.z;
  result.data[ 6] = contact.normal.x;
  result.data[ 7] = contact.normal.y;
  result.data[ 8] = contact.normal.z;
  result.data[ 9] = rn2.x;
  result.data[10] = rn2.y;
  result.data[11] = rn2.z;
  result.data[12 +  0] = -o1.x;
  result.data[12 +  1] = -o1.y;
  result.data[12 +  2] = -o1.z;
  result.data[12 +  3] = -rs1.x;
  result.data[12 +  4] = -rs1.y;
  result.data[12 +  5] = -rs1.z;
  result.data[12 +  6] = o1.x;
  result.data[12 +  7] = o1.y;
  result.data[12 +  8] = o1.z;
  result.data[12 +  9] = rs2.x;
  result.data[12 + 10] = rs2.y;
  result.data[12 + 11] = rs2.z;
  result.data[2 * 12 +  0] = -o2.x;
  result.data[2 * 12 +  1] = -o2.y;
  result.data[2 * 12 +  2] = -o2.z;
  result.data[2 * 12 +  3] = -rt1.x;
  result.data[2 * 12 +  4] = -rt1.y;
  result.data[2 * 12 +  5] = -rt1.z;
  result.data[2 * 12 +  6] = o2.x;
  result.data[2 * 12 +  7] = o2.y;
  result.data[2 * 12 +  8] = o2.z;
  result.data[2 * 12 +  9] = rt2.x;
  result.data[2 * 12 + 10] = rt2.y;
  result.data[2 * 12 + 11] = rt2.z;
  return result;
}

// Compute correction for a contact point of two objects.
// http://image.diku.dk/kenny/download/erleben.05.thesis.pdf
large_vector_t contact_correction(contact_t contact, bool do_restitution) {
  large_vector_t result = allocate_large_vector(3);
  double correction;
  if (do_restitution) {
    // Only spearating collision impulses are allowed.
    correction = contact.normal_speed < 0 ? contact.normal_speed * contact.restitution : 0.0;
  } else {
    // Only separating stabilization is allowed.
    correction = contact.distance < 0 ? contact.distance : 0.0;
  };
  result.data[0] = correction;
  result.data[1] = 0;
  result.data[2] = 0;
  return result;
}

// Compute impulse caused by contact point of two objects.
void contact_impulse(body_t body1, body_t body2, state_t *state1, state_t *state2, contact_t contact,
                     vector_t *impulse1, vector_t *impulse2, vector_t *tau1, vector_t *tau2, bool do_restitution) {
  large_matrix_t j = contact_jacobian(contact, state1, state2);
  large_vector_t b = contact_correction(contact, do_restitution);
  large_vector_t lambda_ = lambda(body1, body2, state1, state2, j, b);
  assert(lambda_.rows == 3);
  // Only separating impulses are allowed.
  if (lambda_.data[0] < 0)
    lambda_.data[0] = 0;
  // Limit friction.
  double f = sqrt(lambda_.data[1] * lambda_.data[1] + lambda_.data[2] * lambda_.data[2]);
  if (f > lambda_.data[0] * 0.1) {
    double c = lambda_.data[0] * 0.1 / f;
    lambda_.data[1] *= c;
    lambda_.data[2] *= c;
  }
  apply_lambda(j, lambda_, impulse1, impulse2, tau1, tau2);
}
