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

// Compute state change for given time interval dt and current state s_.
void *state_change(double t, double dt, void *s_, void *data_) {
  state_t *s = s_;
  body_info_t *data = data_;
  double scale_momentum = dt / data->mass;
  vector_t position_change =
    vector(s->linear_momentum.x * scale_momentum, s->linear_momentum.y * scale_momentum, s->linear_momentum.z * scale_momentum);
  vector_t linear_momentum_change = vector(data->force.x * dt, data->force.y * dt, data->force.z * dt);
  vector_t angular_momentum_change = vector(data->torque.x * dt, data->torque.y * dt, data->torque.z * dt);
  state_t *result = state(position_change, linear_momentum_change, quaternion(0, 0, 0, 0), angular_momentum_change);
  return result;
}
