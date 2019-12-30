#include <assert.h>
#include <stdbool.h>
#include "joint.h"


// Create mass and inertia matrix for a pair of bodies.
large_matrix_t joint_mass(body_t body1, body_t body2, state_t *state1, state_t *state2) {
  large_matrix_t result = allocate_large_matrix(12, 12);
  memset(result.data, 0, result.rows * result.cols * sizeof(double));
  double *p = result.data;
  *p = body1.mass; p += 13; *p = body1.mass; p += 13; *p = body1.mass; p += 13;
  matrix_t inertia1 = rotate_matrix(state1->orientation, body1.inertia);
  *p++ = inertia1.m11; *p++ = inertia1.m12; *p++ = inertia1.m13; p += 9;
  *p++ = inertia1.m21; *p++ = inertia1.m22; *p++ = inertia1.m23; p += 9;
  *p++ = inertia1.m31; *p++ = inertia1.m32; *p++ = inertia1.m33; p += 12;
  *p = body2.mass; p += 13; *p = body2.mass; p += 13; *p = body2.mass; p += 13;
  matrix_t inertia2 = rotate_matrix(state2->orientation, body2.inertia);
  *p++ = inertia2.m11; *p++ = inertia2.m12; *p++ = inertia2.m13; p += 9;
  *p++ = inertia2.m21; *p++ = inertia2.m22; *p++ = inertia2.m23; p += 9;
  *p++ = inertia2.m31; *p++ = inertia2.m32; *p++ = inertia2.m33; p += 12;
  return result;
}

// Create joint speed vector for a pair of bodies.
large_vector_t speed_vector(state_t *state1, state_t *state2) {
  large_vector_t result = allocate_large_vector(12);
  result.data[ 0] = state1->speed.x;
  result.data[ 1] = state1->speed.y;
  result.data[ 2] = state1->speed.z;
  result.data[ 3] = state1->rotation.x;
  result.data[ 4] = state1->rotation.y;
  result.data[ 5] = state1->rotation.z;
  result.data[ 6] = state2->speed.x;
  result.data[ 7] = state2->speed.y;
  result.data[ 8] = state2->speed.z;
  result.data[ 9] = state2->rotation.x;
  result.data[10] = state2->rotation.y;
  result.data[11] = state2->rotation.z;
  return result;
}

// Get multiplier vector for rows of joint or contact Jacobian.
large_vector_t lambda(body_t body1, body_t body2, state_t *state1, state_t *state2, large_matrix_t j, large_vector_t b) {
  large_matrix_t m = joint_mass(body1, body2, state1, state2);
  large_vector_t u = speed_vector(state1, state2);
  large_matrix_t denominator = large_matrix_dot(large_matrix_dot(j, large_inverse(m)), large_transpose(j));
  large_vector_t nominator = large_vector_add(large_matrix_vector_dot(j, u), b);
  return large_vector_negative(large_matrix_vector_dot(large_inverse(denominator), nominator));
}

// Use Jacobian and multiplier vector to compute linear and angular impulses for the two rigid bodies.
void apply_lambda(large_matrix_t j, large_vector_t lambda, vector_t *impulse1, vector_t *impulse2, vector_t *tau1, vector_t *tau2) {
  large_vector_t p = large_matrix_vector_dot(large_transpose(j), lambda);
  assert(p.rows == 12);
  impulse1->x = p.data[0]; impulse1->y = p.data[1]; impulse1->z = p.data[2];
  tau1->x = p.data[3]; tau1->y = p.data[ 4]; tau1->z = p.data[ 5];
  impulse2->x = p.data[6]; impulse2->y = p.data[7]; impulse2->z = p.data[8];
  tau2->x = p.data[9]; tau2->y = p.data[10]; tau2->z = p.data[11];
}

// Determine correcting impulse given the Jacobian and the error vector.
void correcting_impulse(body_t body1, body_t body2, state_t *state1, state_t *state2, large_matrix_t j, large_vector_t b,
                        vector_t *impulse1, vector_t *impulse2, vector_t *tau1, vector_t *tau2) {
  large_vector_t lambda_ = lambda(body1, body2, state1, state2, j, b);
  apply_lambda(j, lambda_, impulse1, impulse2, tau1, tau2);
}

// Construct Jacobian for ball-in-socket joint.
// http://image.diku.dk/kenny/download/erleben.05.thesis.pdf
large_matrix_t ball_in_socket_jacobian(state_t *state1, state_t *state2, ball_in_socket_t joint) {
  large_matrix_t result = allocate_large_matrix(3, 12);
  memset(result.data, 0, result.rows * result.cols * sizeof(double));
  double *p0 = result.data;
  *p0 = 1; p0 += 13; *p0 = 1; p0 += 13; *p0 = 1; p0 += 13;
  matrix_t rot1 = matrix_negative(cross_product_matrix(rotate_vector(state1->orientation, joint.r1)));
  double *p1 = result.data + 3;
  *p1++ = rot1.m11; *p1++ = rot1.m12; *p1++ = rot1.m13; p1 += 9;
  *p1++ = rot1.m21; *p1++ = rot1.m22; *p1++ = rot1.m23; p1 += 9;
  *p1++ = rot1.m31; *p1++ = rot1.m32; *p1++ = rot1.m33; p1 += 9;
  double *p2 = result.data + 6;
  *p2 = -1; p2 += 13; *p2 = -1; p2 += 13; *p2 = -1; p2 += 13;
  matrix_t rot2 = cross_product_matrix(rotate_vector(state2->orientation, joint.r2));
  double *p3 = result.data + 9;
  *p3++ = rot2.m11; *p3++ = rot2.m12; *p3++ = rot2.m13; p3 += 9;
  *p3++ = rot2.m21; *p3++ = rot2.m22; *p3++ = rot2.m23; p3 += 9;
  *p3++ = rot2.m31; *p3++ = rot2.m32; *p3++ = rot2.m33; p3 += 9;
  return result;
}

// Compute error correction term for ball-in-socket joint.
// http://image.diku.dk/kenny/download/erleben.05.thesis.pdf
large_vector_t ball_in_socket_correction(state_t *state1, state_t *state2, ball_in_socket_t joint) {
  return to_large_vector(vector_subtract(vector_add(state1->position, rotate_vector(state1->orientation, joint.r1)),
                                         vector_add(state2->position, rotate_vector(state2->orientation, joint.r2))));
}

static vector_t hinge_axis(state_t *state1, state_t *state2, hinge_t hinge) {
  vector_t s1 = rotate_vector(state1->orientation, hinge.s1);
  vector_t s2 = rotate_vector(state2->orientation, hinge.s2);
  return vector_scale(vector_add(s1, s2), 0.5);
}

// Construct Jacobian for hinge joint.
// http://image.diku.dk/kenny/download/erleben.05.thesis.pdf
large_matrix_t hinge_jacobian(state_t *state1, state_t *state2, hinge_t joint) {
  large_matrix_t result = allocate_large_matrix(5, 12);
  memset(result.data, 0, result.rows * result.cols * sizeof(double));
  vector_t s = hinge_axis(state1, state2, joint);
  vector_t t1 = orthogonal1(s);
  vector_t t2 = orthogonal2(s);
  double *p0 = result.data;
  *p0 = 1; p0 += 13; *p0 = 1; p0 += 13; *p0 = 1; p0 += 13;
  matrix_t rot1 = matrix_negative(cross_product_matrix(rotate_vector(state1->orientation, joint.r1)));
  double *p1 = result.data + 3;
  *p1++ = rot1.m11; *p1++ = rot1.m12; *p1++ = rot1.m13; p1 += 9;
  *p1++ = rot1.m21; *p1++ = rot1.m22; *p1++ = rot1.m23; p1 += 9;
  *p1++ = rot1.m31; *p1++ = rot1.m32; *p1++ = rot1.m33; p1 += 9;
  *p1++ = t1.x; *p1++ = t1.y; *p1++ = t1.z; p1 += 9;
  *p1++ = t2.x; *p1++ = t2.y; *p1++ = t2.z; p1 += 9;
  double *p2 = result.data + 6;
  *p2 = -1; p2 += 13; *p2 = -1; p2 += 13; *p2 = -1; p2 += 13;
  matrix_t rot2 = cross_product_matrix(rotate_vector(state2->orientation, joint.r2));
  double *p3 = result.data + 9;
  *p3++ = rot2.m11; *p3++ = rot2.m12; *p3++ = rot2.m13; p3 += 9;
  *p3++ = rot2.m21; *p3++ = rot2.m22; *p3++ = rot2.m23; p3 += 9;
  *p3++ = rot2.m31; *p3++ = rot2.m32; *p3++ = rot2.m33; p3 += 9;
  *p3++ = -t1.x; *p3++ = -t1.y; *p3++ = -t1.z; p3 += 9;
  *p3++ = -t2.x; *p3++ = -t2.y; *p3++ = -t2.z; p3 += 9;
  return result;
}

// Compute error correction term for hinge joint.
// http://image.diku.dk/kenny/download/erleben.05.thesis.pdf
large_vector_t hinge_correction(state_t *state1, state_t *state2, hinge_t joint) {
  large_vector_t result = allocate_large_vector(5);
  vector_t positional = vector_subtract(vector_add(state1->position, rotate_vector(state1->orientation, joint.r1)),
                                        vector_add(state2->position, rotate_vector(state2->orientation, joint.r2)));
  vector_t s1 = rotate_vector(state1->orientation, joint.s1);
  vector_t s2 = rotate_vector(state2->orientation, joint.s2);
  vector_t u = cross_product(s2, s1);
  vector_t s = hinge_axis(state1, state2, joint);
  vector_t t1 = orthogonal1(s);
  vector_t t2 = orthogonal2(s);
  result.data[0] = positional.x;
  result.data[1] = positional.y;
  result.data[2] = positional.z;
  result.data[3] = inner_product(t1, u);
  result.data[4] = inner_product(t2, u);
  return result;
}

static vector_t slider_axis(state_t *state1, state_t *state2, slider_t slider) {
  vector_t s1 = rotate_vector(state1->orientation, rotate_vector(slider.q1, vector(1, 0, 0)));
  vector_t s2 = rotate_vector(state2->orientation, rotate_vector(slider.q2, vector(1, 0, 0)));
  return vector_scale(vector_add(s1, s2), 0.5);
}

large_matrix_t slider_jacobian(state_t *state1, state_t *state2, slider_t slider) {
  large_matrix_t result = allocate_large_matrix(5, 12);
  memset(result.data, 0, result.rows * result.cols * sizeof(double));
  vector_t ri = rotate_vector(state1->orientation, slider.r1);
  vector_t rj = rotate_vector(state2->orientation, slider.r2);
  vector_t c = vector_subtract(rj, ri);
  vector_t s = slider_axis(state1, state2, slider);
  vector_t t1 = orthogonal1(s);
  vector_t t2 = orthogonal2(s);
  vector_t ct1 = vector_scale(cross_product(c, t1), 0.5);
  vector_t ct2 = vector_scale(cross_product(c, t2), 0.5);
  double *p0 = result.data + 3 * 12;
  *p0++ = t1.x; *p0++ = t1.y; *p0++ = t1.z; p0 += 9;
  *p0++ = t2.x; *p0++ = t2.y; *p0++ = t2.z; p0 += 9;
  double *p1 = result.data + 3;
  *p1 = 1; p1 += 13; *p1 = 1; p1 += 13; *p1 = 1; p1 += 10;
  *p1++ = ct1.x; *p1++ = ct1.y; *p1++ = ct1.z; p1 += 9;
  *p1++ = ct2.x; *p1++ = ct2.y; *p1++ = ct2.z;
  double *p2 = result.data + 6 + 3 * 12;
  *p2++ = -t1.x; *p2++ = -t1.y; *p2++ = -t1.z; p2 += 9;
  *p2++ = -t2.x; *p2++ = -t2.y; *p2++ = -t2.z; p2 += 9;
  double *p3 = result.data + 9;
  *p3 = -1; p3 += 13; *p3 = -1; p3 += 13; *p3 = -1; p3 += 10;
  *p3++ = ct1.x; *p3++ = ct1.y; *p3++ = ct1.z; p3 += 9;
  *p3++ = ct2.x; *p3++ = ct2.y; *p3++ = ct2.z;
  return result;
}

// Determine correcting impulse for a given joint.
void joint_impulse(body_t body1, body_t body2, joint_t joint, state_t *state1, state_t *state2,
                   vector_t *impulse1, vector_t *impulse2, vector_t *tau1, vector_t *tau2) {
  large_matrix_t j;
  large_vector_t b;
  switch (joint.joint_type) {
    case BALL_IN_SOCKET:
      j = ball_in_socket_jacobian(state1, state2, joint.ball_in_socket);
      b = ball_in_socket_correction(state1, state2, joint.ball_in_socket);
      break;
    case HINGE:
      j = hinge_jacobian(state1, state2, joint.hinge);
      b = hinge_correction(state1, state2, joint.hinge);
      break;
    default:
      assert(false);
  };
  correcting_impulse(body1, body2, state1, state2, j, b, impulse1, impulse2, tau1, tau2);
}
