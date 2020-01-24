#include <assert.h>
#include <tgmath.h>
#include "friction.h"


// Limit friction to stay inside friction cone.
// http://myselph.de/gamePhysics/friction.html
large_vector_t limit_friction(friction_t friction, double contact_lambda, large_vector_t lambda) {
  large_vector_t result;
  assert(lambda.rows == 2);
  double norm;
  switch (friction.type) {
    case UNIFORM_FRICTION:
      norm = sqrt(lambda.data[0] * lambda.data[0] + lambda.data[1] * lambda.data[1]);
      if (norm > contact_lambda * friction.uniform) {
        result = allocate_large_vector(2);
        result.data[0] = lambda.data[0] * contact_lambda * friction.uniform / norm;
        result.data[1] = lambda.data[1] * contact_lambda * friction.uniform / norm;
      } else
        result = lambda;
      break;
    case ANISOTROPIC_FRICTION:
      result = allocate_large_vector(2);
      if (fabs(lambda.data[0]) > contact_lambda * friction.anisotropic.value1)
        result.data[0] = lambda.data[0] * contact_lambda * friction.anisotropic.value1 / fabs(lambda.data[0]);
      else
        result.data[0] = lambda.data[0];
      if (fabs(lambda.data[1]) > contact_lambda * friction.anisotropic.value2)
        result.data[1] = lambda.data[1] * contact_lambda * friction.anisotropic.value2 / fabs(lambda.data[1]);
      else
        result.data[1] = lambda.data[1];
      break;
    default:
      assert(false);
  };
  return result;
}
