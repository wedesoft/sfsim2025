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
  double *p0 = result.data;
  *p0++ = -contact.normal.x; *p0++ = -contact.normal.y; *p0++ = -contact.normal.z; p0+= 9;
  double *p1 = result.data + 3;
  *p1++ = -rn1.x; *p1++ = -rn1.y; *p1++ = -rn1.z; p1 += 9;
  double *p2 = result.data + 6;
  *p2++ = contact.normal.x; *p2++ = contact.normal.y; *p2++ = contact.normal.z; p2+= 9;
  double *p3 = result.data + 9;
  *p3++ = rn2.x; *p3++ = rn2.y; *p3++ = rn2.z; p3 += 9;
  return result;
}

// Compute correction for a contact point of two objects.
// http://image.diku.dk/kenny/download/erleben.05.thesis.pdf
large_vector_t contact_correction(contact_t contact, bool do_restitution) {
  large_vector_t result = allocate_large_vector(1);
  double correction;
  if (do_restitution) {
    // Only spearating collision impulses are allowed.
    correction = contact.normal_speed < 0 ? contact.normal_speed * contact.restitution : 0.0;
  } else {
    // Only separating stabilization is allowed.
    correction = contact.distance < 0 ? contact.distance : 0.0;
  };
  result.data[0] = correction;
  return result;
}

large_matrix_t friction_jacobian(contact_t contact, state_t *state1, state_t *state2) {
  large_matrix_t result = allocate_large_matrix(2, 12);
  vector_t u = orthogonal1(contact.normal);
  vector_t v = orthogonal2(contact.normal);
  vector_t r1 = vector_subtract(contact.point, state1->position);
  vector_t r2 = vector_subtract(contact.point, state2->position);
  vector_t ru1 = cross_product(r1, u);
  vector_t ru2 = cross_product(r2, u);
  vector_t rv1 = cross_product(r1, v);
  vector_t rv2 = cross_product(r2, v);
  double *p0 = result.data;
  *p0++ = u.x; *p0++ = u.y; *p0++ = u.z; p0 += 9;
  *p0++ = v.x; *p0++ = v.y; *p0++ = v.z; p0 += 9;
  double *p1 = result.data + 3;
  *p1++ = ru1.x; *p1++ = ru1.y; *p1++ = ru1.z; p1 += 9;
  *p1++ = rv1.x; *p1++ = rv1.y; *p1++ = rv1.z; p1 += 9;
  double *p2 = result.data + 6;
  *p2++ = -u.x; *p2++ = -u.y; *p2++ = -u.z; p2 += 9;
  *p2++ = -v.x; *p2++ = -v.y; *p2++ = -v.z; p2 += 9;
  double *p3 = result.data + 9;
  *p3++ = -ru2.x; *p3++ = -ru2.y; *p3++ = -ru2.z; p3 += 9;
  *p3++ = -rv2.x; *p3++ = -rv2.y; *p3++ = -rv2.z; p3 += 9;
  return result;
}

large_vector_t friction_correction(contact_t contact) {
  large_vector_t result = allocate_large_vector(2);
  double correction;
  result.data[1] = 0;
  result.data[2] = 0;
  return result;
}
// Compute impulse caused by contact point of two objects.
void contact_impulse(body_t body1, body_t body2, state_t *state1, state_t *state2, contact_t contact,
                     vector_t *impulse1, vector_t *impulse2, vector_t *tau1, vector_t *tau2, bool do_restitution) {
  large_matrix_t contact_j = contact_jacobian(contact, state1, state2);
  large_vector_t contact_b = contact_correction(contact, do_restitution);
  large_vector_t lambda_contact = lambda(body1, body2, state1, state2, contact_j, contact_b);
  large_matrix_t friction_j = friction_jacobian(contact, state1, state2);
  large_vector_t b2 = friction_correction(contact);
  large_vector_t lambda_friction = lambda(body1, body2, state1, state2, friction_j, b2);
  // Only separating impulses are allowed.
  if (lambda_contact.data[0] < 0)
    lambda_contact.data[0] = 0;
  double friction = sqrt(lambda_friction.data[0] * lambda_friction.data[0] + lambda_friction.data[1] * lambda_friction.data[1]);
  if (friction > lambda_contact.data[0] * contact.friction) {
    double factor = lambda_contact.data[0] * contact.friction / friction;
    lambda_friction.data[0] *= factor;
    lambda_friction.data[1] *= factor;
  };
  vector_t contact_impulse1, contact_impulse2, contact_tau1, contact_tau2;
  apply_lambda(contact_j, lambda_contact, &contact_impulse1, &contact_impulse2, &contact_tau1, &contact_tau2);
  vector_t friction_impulse1, friction_impulse2, friction_tau1, friction_tau2;
  apply_lambda(friction_j, lambda_friction, &friction_impulse1, &friction_impulse2, &friction_tau1, &friction_tau2);
  *impulse1 = vector_add(contact_impulse1, friction_impulse1);
  *impulse2 = vector_add(contact_impulse2, friction_impulse2);
  *tau1 = vector_add(contact_tau1, friction_tau1);
  *tau2 = vector_add(contact_tau2, friction_tau2);
}
