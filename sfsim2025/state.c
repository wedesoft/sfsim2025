#include "state.h"


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
