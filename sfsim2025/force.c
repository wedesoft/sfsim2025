#include <assert.h>
#include "force.h"


// Compute forces and torques.
void exert_force(body_t body1, body_t body2, force_t force, state_t *state1, state_t *state2,
                 vector_t *force1, vector_t *force2, vector_t *tau1, vector_t *tau2) {
  assert(force.force_type == GRAVITATION);
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
