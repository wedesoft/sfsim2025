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
  large_matrix_t result = allocate_large_matrix(1, 12);
  vector_t r1 = vector_subtract(contact.point, state1->position);
  vector_t r2 = vector_subtract(contact.point, state2->position);
  vector_t rn1 = cross_product(r1, contact.normal);
  vector_t rn2 = cross_product(r2, contact.normal);
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
  return result;
}

// Compute correction for a contact point of two objects.
// http://image.diku.dk/kenny/download/erleben.05.thesis.pdf
large_vector_t contact_correction(contact_t contact, state_t *state1, state_t *state2) {
  large_vector_t result = allocate_large_vector(1);
  vector_t r1 = vector_subtract(contact.point, state1->position);
  vector_t r2 = vector_subtract(contact.point, state2->position);
  vector_t angular_speed1 = cross_product(state1->rotation, r1);
  vector_t angular_speed2 = cross_product(state2->rotation, r2);
  vector_t v_rel = relative_speed(state1, state2, contact.point);
  double v_normal = inner_product(v_rel, contact.normal);
  // Only separating corrections are allowed.
  double correction = contact.distance < 0 ? contact.distance : 0.0;
  double restitution = v_normal < 0 ? v_normal * contact.restitution : 0.0;
  result.data[0] = correction + restitution;
  return result;
}

// Compute impulse caused by contact point of two objects.
void contact_impulse(body_t body1, body_t body2, state_t *state1, state_t *state2, contact_t contact,
                     vector_t *impulse1, vector_t *impulse2, vector_t *tau1, vector_t *tau2) {
  large_matrix_t j = contact_jacobian(contact, state1, state2);
  large_vector_t b = contact_correction(contact, state1, state2);
  large_vector_t lambda_ = lambda(body1, body2, state1, state2, j, b);
  assert(lambda_.rows == 1);
  // Only separating impulses are allowed.
  if (lambda_.data[0] < 0)
    lambda_.data[0] = 0;
  apply_lambda(j, lambda_, impulse1, impulse2, tau1, tau2);
}
