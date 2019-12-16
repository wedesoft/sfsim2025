#include "mechanics.h"

// https://en.wikipedia.org/wiki/Newton%E2%80%93Euler_equations
// https://en.wikipedia.org/wiki/Euler%27s_equations_(rigid_body_dynamics)
// http://www.kuffner.org/james/software/dynamics/mirtich/mirtichThesis.pdf


// Fourth order Runge-Kutta integration.
// https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods
void *runge_kutta(void *y0, double dt, void *f(double, double, void *, void *), void *add(void *, void *),
                  void *scale(void *, double), void *data) {
  double dt2 = 0.5 * dt;
  void *k1 = f(0  , dt, y0                     , data);
  void *k2 = f(dt2, dt, add(y0, scale(k1, 0.5)), data);
  void *k3 = f(dt2, dt, add(y0, scale(k2, 0.5)), data);
  void *k4 = f(dt , dt, add(y0, k3            ), data);
  return add(y0, scale(add(add(add(k1, scale(k2, 2)), scale(k3, 2)), k4), 1.0 / 6.0));
}

// Predict speed and rotation for next time step.
state_t *predict(state_t *s, body_t b, forces_t f, vector_t p, vector_t t, double dt) {
  vector_t speed = vector_add(s->speed, speed_change(b, f.force, p, dt));
  vector_t rotation = vector_add(s->rotation, rotation_change(s, f, b, t, dt));
  return state(s->position, speed, s->orientation, rotation);
}
