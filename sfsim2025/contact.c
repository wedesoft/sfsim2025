#include "contact.h"


// Construct Jacobian for a contact point of two objects.
// http://image.diku.dk/kenny/download/erleben.05.thesis.pdf
large_matrix_t contact_jacobian(contact_t contact, state_t *state1, state_t *state2) {
  large_matrix_t result = allocate_large_matrix(1, 12);
  vector_t r1 = vector_subtract(contact.point, state1->position);
  vector_t r2 = vector_subtract(contact.point, state2->position);
  vector_t rn1 = cross_product(r1, contact.normal);
  vector_t rn2 = cross_product(r2, contact.normal);
  result.data[ 0] = -contact.normal.x;
  result.data[ 1] = -contact.normal.y;
  result.data[ 2] = -contact.normal.z;
  result.data[ 3] = -rn1.x;
  result.data[ 4] = -rn1.y;
  result.data[ 5] = -rn1.z;
  result.data[ 6] = contact.normal.x;
  result.data[ 7] = contact.normal.y;
  result.data[ 8] = contact.normal.z;
  result.data[ 9] = rn2.x;
  result.data[10] = rn2.y;
  result.data[11] = rn2.z;
  return result;
}

large_vector_t contact_correction(contact_t contact) {
  large_vector_t result = allocate_large_vector(1);
  result.data[0] = contact.distance < 0 ? -contact.distance : 0.0;
  return result;
}
