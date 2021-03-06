#include <assert.h>
#include "mechanics.h"
#include "state.h"
#include "world.h"


void *scale_world(void *world_, double scale) {
  world_t *world = world_;
  world_t *result = make_world();
  for (int i=0; i<world->states.size; i++) {
    state_t *state = get_pointer(world->states)[i];
    append_pointer(&result->states, scale_state(state, scale));
  };
  return result;
}

void *add_worlds(void *a_, void *b_) {
  world_t *a = a_;
  world_t *b = b_;
  world_t *result = make_world();
  assert(a->states.size == b->states.size);
  for (int i=0; i<a->states.size; i++) {
    state_t *s1 = get_pointer(a->states)[i];
    state_t *s2 = get_pointer(b->states)[i];
    append_pointer(&result->states, add_states(s1, s2));
  };
  return result;
}

state_t *state_change(state_t *s, body_t b, vector_t force, vector_t torque, vector_t linear_impulse, vector_t angular_impulse,
                      double dt) {
  vector_t position_change = vector_scale(s->speed, dt);
  vector_t speed_change_ = speed_change(b, force, linear_impulse, dt);
  quaternion_t orientation_change = quaternion_product(vector_to_quaternion(vector_scale(s->rotation, 0.5 * dt)), s->orientation);
  vector_t rotation_change_ = rotation_change(s, b, torque, angular_impulse, dt);
  return state(position_change, speed_change_, orientation_change, rotation_change_);
}

static inline list_t transform_bodies(list_t bodies, list_t states) {
  list_t result = make_list();
  for (int i=0; i<bodies.size; i++) {
    hull_t *hull = get_pointer(bodies)[i];
    state_t *state = get_pointer(states)[i];
    append_pointer(&result, transform_body(hull, state->orientation, state->position));
  };
  return result;
}

static inline list_t determine_contacts(list_t bodies, list_t states, list_t contact_candidates, double restitution) {
  list_t result = make_list();
  for (int k=0; k<contact_candidates.size; k++) {
    contact_candidate_t candidate = get_contact_candidate(contact_candidates)[k];
    int i = candidate.i;
    int j = candidate.j;
    hull_t *body = get_pointer(bodies)[i];
    hull_t *other = get_pointer(bodies)[j];
    state_t *state1 = get_pointer(states)[i];
    state_t *state2 = get_pointer(states)[j];
    list_t contacts_of_bodies = contacts(i, j, body, other, restitution, candidate.friction, state1, state2);
    for (int c=0; c<contacts_of_bodies.size; c++)
      append_contact(&result, get_contact(contacts_of_bodies)[c]);
  };
  return result;
}

void *world_change(double time, double dt, void *world_, void *data_) {
  world_t *world = world_;
  world_t *result = make_world();
  world_info_t *data = data_;
  assert(data->bodies.size == world->states.size);
  assert(data->rigid_bodies.size == world->states.size);
  // Rotate and translate the rigid bodies.
  list_t rigid_bodies = transform_bodies(data->rigid_bodies, world->states);
  // Determine all contacts between rigid bodies.
  list_t contacts_ = determine_contacts(rigid_bodies, world->states, data->contact_candidates, data->restitution);
  // Initialise forces and torques to zero.
  vector_t force[data->bodies.size]; memset(force, 0, sizeof(force));
  vector_t torque[data->bodies.size]; memset(torque, 0, sizeof(torque));
  // Initialise linear and rotational impulses to zero.
  vector_t linear_impulse[world->states.size]; memset(linear_impulse, 0, sizeof(linear_impulse));
  vector_t angular_impulse[world->states.size]; memset(angular_impulse, 0, sizeof(angular_impulse));
  vector_t l_i_contact1[contacts_.size]; memset(l_i_contact1, 0, sizeof(l_i_contact1));
  vector_t l_i_contact2[contacts_.size]; memset(l_i_contact2, 0, sizeof(l_i_contact2));
  vector_t a_i_contact1[contacts_.size]; memset(a_i_contact1, 0, sizeof(a_i_contact1));
  vector_t a_i_contact2[contacts_.size]; memset(a_i_contact2, 0, sizeof(a_i_contact2));
  vector_t l_i_friction1[contacts_.size]; memset(l_i_friction1, 0, sizeof(l_i_friction1));
  vector_t l_i_friction2[contacts_.size]; memset(l_i_friction2, 0, sizeof(l_i_friction2));
  vector_t a_i_friction1[contacts_.size]; memset(a_i_friction1, 0, sizeof(a_i_friction1));
  vector_t a_i_friction2[contacts_.size]; memset(a_i_friction2, 0, sizeof(a_i_friction2));
  // Determine forces and torques.
  if (dt > 0) {  // Only required if time step is not zero.
    for (int k=0; k<data->forces.size; k++) {
      force_t f = get_force(data->forces)[k];
      int i = f.i;
      int j = f.j;
      state_t *s1 = get_pointer(world->states)[i];
      state_t *s2 = get_pointer(world->states)[j];
      body_t body1 = get_body(data->bodies)[i];
      body_t body2 = get_body(data->bodies)[j];
      vector_t force1; vector_t force2; vector_t tau1; vector_t tau2;
      exert_force(body1, body2, f, s1, s2, &force1, &force2, &tau1, &tau2);
      force[i] = vector_add(force[i], force1);
      force[j] = vector_add(force[j], force2);
      torque[i] = vector_add(torque[i], tau1);
      torque[j] = vector_add(torque[j], tau2);
    };
  };
  // Perform sequential impulses for several iterations.
  for (int c=0; c<data->iterations; c++) {
    // Apply impulses generated by joints.
    for (int k=0; k<data->joints.size; k++) {
      // Get participants of joint constraint.
      joint_t joint = get_joint(data->joints)[k];
      int i = joint.i;
      int j = joint.j;
      state_t *s1 = get_pointer(world->states)[i];
      state_t *s2 = get_pointer(world->states)[j];
      body_t body1 = get_body(data->bodies)[i];
      body_t body2 = get_body(data->bodies)[j];
      state_t *prediction1 = predict(s1, body1, force[i], torque[i], linear_impulse[i], angular_impulse[i], dt);
      state_t *prediction2 = predict(s2, body2, force[j], torque[j], linear_impulse[j], angular_impulse[j], dt);
      vector_t p1; vector_t p2; vector_t t1; vector_t t2;
      joint_impulse(body1, body2, joint, prediction1, prediction2, &p1, &p2, &t1, &t2);
      // Update joint impulse.
      linear_impulse[i] = vector_add(linear_impulse[i], p1);
      linear_impulse[j] = vector_add(linear_impulse[j], p2);
      angular_impulse[i] = vector_add(angular_impulse[i], t1);
      angular_impulse[j] = vector_add(angular_impulse[j], t2);
    };
    // Apply impulses generated by contacts.
    for (int k=0; k<contacts_.size; k++) {
      contact_t c = get_contact(contacts_)[k];
      // Select resting contacts if dt > 0, select collision contacts if dt = 0.
      if (is_resting(c, data->resting_threshold) == (dt != 0)) {
        // Get participants of contact constraint.
        int i = c.i;
        int j = c.j;
        state_t *s1 = get_pointer(world->states)[i];
        state_t *s2 = get_pointer(world->states)[j];
        body_t body1 = get_body(data->bodies)[i];
        body_t body2 = get_body(data->bodies)[j];
        // Update contact impulse
        linear_impulse[i] = vector_subtract(linear_impulse[i], l_i_contact1[k]);
        linear_impulse[j] = vector_subtract(linear_impulse[j], l_i_contact2[k]);
        angular_impulse[i] = vector_subtract(angular_impulse[i], a_i_contact1[k]);
        angular_impulse[j] = vector_subtract(angular_impulse[j], a_i_contact2[k]);
        state_t *contact_prediction1 = predict(s1, body1, force[i], torque[i], linear_impulse[i], angular_impulse[i], dt);
        state_t *contact_prediction2 = predict(s2, body2, force[j], torque[j], linear_impulse[j], angular_impulse[j], dt);
        double contact_lambda;
        contact_impulse(body1, body2, contact_prediction1, contact_prediction2, c, dt == 0.0,
                        &l_i_contact1[k], &l_i_contact2[k], &a_i_contact1[k], &a_i_contact2[k], &contact_lambda);
        linear_impulse[i] = vector_add(linear_impulse[i], l_i_contact1[k]);
        linear_impulse[j] = vector_add(linear_impulse[j], l_i_contact2[k]);
        angular_impulse[i] = vector_add(angular_impulse[i], a_i_contact1[k]);
        angular_impulse[j] = vector_add(angular_impulse[j], a_i_contact2[k]);
        // Update friction impulse
        linear_impulse[i] = vector_subtract(linear_impulse[i], l_i_friction1[k]);
        linear_impulse[j] = vector_subtract(linear_impulse[j], l_i_friction2[k]);
        angular_impulse[i] = vector_subtract(angular_impulse[i], a_i_friction1[k]);
        angular_impulse[j] = vector_subtract(angular_impulse[j], a_i_friction2[k]);
        state_t *friction_prediction1 = predict(s1, body1, force[i], torque[i], linear_impulse[i], angular_impulse[i], dt);
        state_t *friction_prediction2 = predict(s2, body2, force[j], torque[j], linear_impulse[j], angular_impulse[j], dt);
        friction_impulse(body1, body2, friction_prediction1, friction_prediction2, c,
                         &l_i_friction1[k], &l_i_friction2[k], &a_i_friction1[k], &a_i_friction2[k], contact_lambda);
        linear_impulse[i] = vector_add(linear_impulse[i], l_i_friction1[k]);
        linear_impulse[j] = vector_add(linear_impulse[j], l_i_friction2[k]);
        angular_impulse[i] = vector_add(angular_impulse[i], a_i_friction1[k]);
        angular_impulse[j] = vector_add(angular_impulse[j], a_i_friction2[k]);
      };
    };
  };
  // Create resulting list of state changes.
  for (int i=0; i<world->states.size; i++) {
    state_t *s = get_pointer(world->states)[i];
    body_t b = get_body(data->bodies)[i];
    state_t *change = state_change(s, b, force[i], torque[i], linear_impulse[i], angular_impulse[i], dt);
    append_pointer(&result->states, change);
  };
  return result;
}
