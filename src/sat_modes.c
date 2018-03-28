#include "sat_modes.h"
#include "satellite.h"

static struct _sat_state sat_state = {
  .current_mode = ACTIVATION_MODE_ID,
  .previous_mode = ACTIVATION_MODE_ID
};

void satellite_mode() {

  bool transit = false;
  sat_mode_id next_mode = ACTIVATION_MODE_ID;

  if(sat_state.current_mode == ACTIVATION_MODE_ID) {

    bool elapsed_time = 0;

    elapsed_time = sat_elapsed_time();

    transit = activation_mode(&next_mode,
                              sat_submode_act_id state,
                              sat_submode_act_id next_state,
                              time_elapsed);

  } else if(sat_state.current_mode == DEPLOYMENT_MODE_ID) {

      bool elapsed_time = false;
      bool deployed = false;

      sat_mode_id next_mode = DEPLOYMENT_MODE_ID;

      elapsed_time = sat_elapsed_time();
      deployed = sat_deployed();

      transit = deployment_mode(&next_mode,
                                sat_submode_act_id state,
                                sat_submode_act_id next_state,
                                time_elapsed);

  } else if(sat_state.current_mode == SAFE_MODE_ID) {

      bool batt_charged = false;
      sat_mode_id next_mode = SAFE_MODE_ID;

      batt_charged = sat_batt_charged();

      transit = safe_mode(&next_mode, batt_charged);
  }

  if(transit == true) {
    change_mode(next_mode);
  }

}

void change_mode(const sat_mode_id next_mode) {

  if(transition_check_sat_modes(sat_state.current_mode, next_mode) == true) {
         sat_state.previous_mode = sat_state.current_mode;
         sat_state.current_mode = next_mode;
  }
}

sat_mode_id get_satellite_mode() {
  return sat_state.current_mode;
}

bool transition_check_sat_modes(sat_mode_id current, sat_mode_id next) {

  if(!C_ASSERT(current < LAST_MODE_ID) == true) {
    return false;
  }

  if(!C_ASSERT(next < LAST_MODE_ID) == true) {
    return false;
  }

  if(!C_ASSERT(current == ACTIVATION_MODE_ID &&
           (next == DEPLOYMENT_MODE_ID || next == OFF_MODE_ID)) == true) {
             return false;
  }

  if(!C_ASSERT(current == DEPLOYMENT_MODE_ID &&
           (next == SAFE_MODE_ID || next == OFF_MODE_ID)) == true) {
             return false;
  }

  if(!C_ASSERT(current == SAFE_MODE_ID && next == ADCS_MODE_ID) == true) {
    return false;
  }

  if(!C_ASSERT(current == ADCS_MODE_ID &&
           (next == NOMINAL_MODE_ID ||
            next == SAFE_MODE_ID ||
            next == OFF_MODE_ID)) == true) {
    return false;
  }

  if(!C_ASSERT(current == NOMINAL_MODE_ID &&
           (next == SAFE_MODE_ID || next == OFF_MODE_ID)) == true) {
    return false;
  }

  return true;
}

bool sat_elapsed_time() {

  bool elapsed = false;
  uint32_t timer = 0;

  get_parameter(EPS_ELAPSED_TIME_PAR_ID, );
  if(timer > SAT_ACTIVATION_TIME) {
    elapsed = true;
  }

  return elapsed;
}

bool sat_batt_ready() {

  bool ready = false;
  float volt = 0.0;
  float threshold = 0.0;

  get_parameter(EPS_BATT_VOLT_PAR_ID, );
  get_parameter(EPS_BATT_VOLT_THRES_PAR_ID, );

  if(volt > threshold) {
    ready = true;
  }

  return ready;
}

bool sat_batt_charged() {

  bool ready = false;
  float volt = 0.0;
  float threshold = 0.0;

  get_parameter(EPS_BATT_VOLT_PAR_ID, );
  get_parameter(EPS_BATT_SAFE_THRES_PAR_ID, );

  if(volt > threshold) {
    ready = true;
  }

  return ready;
}

bool activation_mode(sat_mode_id *next_mode,
                     sat_submode_act_id state,
                     sat_submode_act_id next_state,
                     bool time_elapsed) {

  bool transition = false;

  if(!C_ASSERT(current < LAST_ACT_ID) == true) {
    //error
    return false;
  }

  if(state == ACTIVATED_ACT_ID) {

    transition = true;
    *next_mode = DEPLOYMENT_MODE_ID;

  } else if(state == NOT_ACTIVATED_ACT_ID && time_elapsed == true) {

    transition = true;
    *next_mode = DEPLOYMENT_MODE_ID;

  } else if(state == NOT_ACTIVATED_ACT_ID && time_elapsed == false) {

    transition = false;
    *next_mode = ACTIVATION_MODE_ID;

  } else {
    //error
  }

  return transition;
}

bool deployment_mode(sat_mode_id *next_mode,
                     sat_submode_act_id state,
                     sat_submode_act_id next_state,
                     bool batt_ok,
                     bool time_elapsed) {

  bool transition = false;

  if(!C_ASSERT(current < LAST_DEP_ID) == true) {
    //error
    return false;
  }

  if(state == DEPLOYED_DEP_ID) {

    transition = true;
    *next_mode = SAFE_MODE_ID;

  } else if(state == NOT_DEPLOYED_DEP_ID && batt_ok == true) {

    transition = false;

  } else if(state == NOT_DEPLOYED_DEP_ID && batt_ok == true) {

    transition = false;

  } else if(state == WAIT_DEPLOYED_DEP_ID && time_elapsed == false) {

    transition = false;

  } else {
    //error
  }

  return transition;
}

bool safe_mode(sat_mode_id *next_mode,
               bool batt_charged) {

  bool transition = false;

  if(!C_ASSERT(current < LAST_ACT_ID) == true) {
    //error
    return false;
  }

  if(batt_charged == true) {

    transition = true;
    *next_mode = ADCS_MODE_ID;

  } else {

    transition = false;
  }

  return transition;
}
