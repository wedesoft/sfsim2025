#include <assert.h>
#include "force.h"


static inline void exert_gravitation(body_t body1, body_t body2, state_t *state1, state_t *state2,
                                     vector_t *force1, vector_t *force2, vector_t *tau1, vector_t *tau2) {
  vector_t v = vector_subtract(state2->position, state1->position);
  double r2 = v.x * v.x + v.y * v.y + v.z * v.z;
  double r = sqrt(r2);
  double f = body1.mass * body2.mass * 6.67408e-11 / r2;
  force1->x = f * v.x / r;
  force1->y = f * v.y / r;
  force1->z = f * v.z / r;
  force2->x = -f * v.x / r;
  force2->y = -f * v.y / r;
  force2->z = -f * v.z / r;
  *tau1 = vector(0, 0, 0);
  *tau2 = vector(0, 0, 0);
}

static inline void exert_fixed_force(body_t body1, body_t body2, fixed_force_t fixed_force, state_t *state1, state_t *state2,
                                     vector_t *force1, vector_t *force2, vector_t *tau1, vector_t *tau2) {
  *force1 = rotate_vector(state1->orientation, fixed_force.force);
  *force2 = vector(0, 0, 0);
  *tau1 = rotate_vector(state1->orientation, fixed_force.torque);
  *tau2 = vector(0, 0, 0);
}

// Compute forces and torques.
void exert_force(body_t body1, body_t body2, force_t force, state_t *state1, state_t *state2,
                 vector_t *force1, vector_t *force2, vector_t *tau1, vector_t *tau2) {
  switch (force.force_type) {
    case GRAVITATION:
      exert_gravitation(body1, body2, state1, state2, force1, force2, tau1, tau2);
      break;
    case FIXED_FORCE:
      exert_fixed_force(body1, body2, force.fixed_force, state1, state2, force1, force2, tau1, tau2);
      break;
    default:
      assert(false);
  };
}
