#include "mechanics.h"

// https://en.wikipedia.org/wiki/Newton%E2%80%93Euler_equations
// https://en.wikipedia.org/wiki/Euler%27s_equations_(rigid_body_dynamics)
// http://www.kuffner.org/james/software/dynamics/mirtich/mirtichThesis.pdf


// Fourth order Runge-Kutta integration.
// https://en.wikipedia.org/wiki/Runge%E2%80%93Kutta_methods
void *runge_kutta(void *y0, double dt, void *f(double, void *), void *add(void *, void *), void *scale(void *, double)) {
  double dt2 = 0.5 * dt;
  void *k1 = f(0  , y0                     );
  void *k2 = f(dt2, add(y0, scale(k1, dt2)));
  void *k3 = f(dt2, add(y0, scale(k2, dt2)));
  void *k4 = f(dt , add(y0, scale(k3, dt )));
  return add(y0, scale(add(add(add(k1, scale(k2, 2)), scale(k3, 2)), k4), dt / 6.0));
}
