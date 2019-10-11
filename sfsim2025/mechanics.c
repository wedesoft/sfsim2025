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
  double dt_div_mass = dt / data->mass;
  vector_t position_change = vector_scale(s->speed, dt);
  vector_t speed_change = vector_scale(data->force, dt_div_mass);
  // https://www.cl.cam.ac.uk/techreports/UCAM-CL-TR-683.pdf section 2.3.3
  quaternion_t orientation_change = quaternion_product(vector_to_quaternion(vector_scale(s->rotation, 0.5 * dt)), s->orientation);
  matrix_t inertia = rotate_matrix(s->orientation, data->inertia);
  vector_t coriolis = vector_negative(cross_product(s->rotation, matrix_vector_dot(inertia, s->rotation)));
  vector_t rotation_change = vector_scale(matrix_vector_dot(inverse(inertia), vector_add(data->torque, coriolis)), dt);
  // https://physics.stackexchange.com/questions/412181/eulers-equation-for-rigid-body-rotation-applied-to-inertia-frame
  state_t *result = state(position_change, speed_change, orientation_change, rotation_change);
  return result;
}

// Add two states.
void *add_states(void *a_, void *b_) {
  state_t *a = a_;
  state_t *b = b_;
  return state(vector_add(a->position, b->position),
               vector_add(a->speed, b->speed),
               quaternion_add(a->orientation, b->orientation),
               vector_add(a->rotation, b->rotation));
}

// Scale a state.
void *scale_state(void *s_, double scale) {
  state_t *s = s_;
  return state(vector_scale(s->position, scale),
               vector_scale(s->speed, scale),
               quaternion_scale(s->orientation, scale),
               vector_scale(s->rotation, scale));
}
