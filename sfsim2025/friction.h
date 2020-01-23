#pragma once


typedef enum {UNIFORM_FRICTION, ANISOTROPIC_FRICTION} friction_type_t;

typedef struct {
  double value1;
  double value2;
} friction_anisotropic_t;

typedef struct {
  friction_type_t type;
  union {
    double uniform;
    friction_anisotropic_t anisotropic;
  };
} friction_t;

static inline friction_t uniform_friction(double uniform) {
  return (friction_t){.type=UNIFORM_FRICTION, .uniform=uniform};
}

static inline friction_t anisotropic_friction(double value1, double value2) {
  return (friction_t){.type=ANISOTROPIC_FRICTION, .anisotropic.value1=value1, .anisotropic.value2=value2};
}
