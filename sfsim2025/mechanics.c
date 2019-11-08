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
  double dt_div_mass = dt / data->body.mass;
  vector_t position_change = vector_scale(s->speed, dt);
  vector_t speed_change_ = speed_change(data->forces, data->body, vector(0, 0, 0), dt);
  // https://www.cl.cam.ac.uk/techreports/UCAM-CL-TR-683.pdf section 2.3.3
  quaternion_t orientation_change = quaternion_product(vector_to_quaternion(vector_scale(s->rotation, 0.5 * dt)), s->orientation);
  vector_t rotation_change_ = rotation_change(s, data->forces, data->body, vector(0, 0, 0), dt);
  // https://physics.stackexchange.com/questions/412181/eulers-equation-for-rigid-body-rotation-applied-to-inertia-frame
  state_t *result = state(position_change, speed_change_, orientation_change, rotation_change_);
  return result;
}

// Predict speed and rotation for next time step.
state_t *predict(state_t *s, body_t b, forces_t f, vector_t p, vector_t t, double dt) {
  vector_t speed = vector_add(s->speed, speed_change(f, b, p, dt));
  vector_t rotation = vector_add(s->rotation, rotation_change(s, f, b, t, dt));
  return state(s->position, speed, s->orientation, rotation);
}
