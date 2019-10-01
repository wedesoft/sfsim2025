#pragma once


void *runge_kutta(double t0, void *y0, double dt, void *f(double, void *), void *add(void *, void *),
                  void *scale(void *, double));
