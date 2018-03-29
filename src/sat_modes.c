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
    bool activated_flag =false;
    bool set_activated_flag = false;

    get_parameter(SAT_ACTIVATED_FLAG, &activated_flag);
    elapsed_time = sat_elapsed_time();

    transit = activation_mode(&next_mode,
                              activated_flag,
                              time_elapsed,
                              &set_activated_flag);

    if(set_activated_flag == true) {
      set_parameter(SAT_ACTIVATED_FLAG, true);
    }

  } else if(sat_state.current_mode == DEPLOYMENT_MODE_ID) {

      bool elapsed_time = false;
      bool deployed = false;

      sat_mode_id next_mode = DEPLOYMENT_MODE_ID;

      elapsed_time = sat_elapsed_time();
      deployed = sat_deployed();

      transit = deployment_mode(&next_mode, time_elapsed);

  } else if(sat_state.current_mode == SAFE_MODE_ID) {

    bool batt_charged = false;
    sat_mode_id next_mode = SAFE_MODE_ID;

    batt_charged = sat_batt_charged();

    transit = safe_mode(&next_mode, batt_charged);

  } else if(sat_state.current_mode == ADCS_MODE_ID) {

    bool elapsed_time = false;
    bool deployed = false;
    bool adcs_enabled_flag = true;
    bool first_detumbl_flag = false;
    bool rotation_ok = false;

    sat_mode_id next_mode = DEPLOYMENT_MODE_ID;

    elapsed_time = sat_elapsed_time();
    deployed = sat_deployed();
    rotation_ok = sat_rotation();

    get_parameter(ADCS_ENABLED_FLAG, &adcs_enabled_flag);
    get_parameter(ADCS_ENABLED_FLAG, &first_detumbl_flag);

    transit = adcs_mode(&next_mode,
                        adcs_enabled_flag,
                        const bool first_detumbl_flag,
                        const bool rotation_ok,
                        bool time_elapsed,
                        bool *set_first_detumbl_flag,
                        bool *set_detumbl_flag_to);

  } else if(sat_state.current_mode == NOMINAL_MODE_ID) {

    bool elapsed_time = false;
    bool deployed = false;
    bool adcs_enabled_flag = true;
    bool first_detumbl_flag = false;
    bool rotation_ok = false;

    sat_mode_id next_mode = DEPLOYMENT_MODE_ID;

    elapsed_time = sat_elapsed_time();
    deployed = sat_deployed();
    rotation_ok = sat_rotation();

    get_parameter(ADCS_ENABLED_FLAG, &adcs_enabled_flag);
    get_parameter(ADCS_ENABLED_FLAG, &first_detumbl_flag);

    transit = nominal_mode(&next_mode,
                        adcs_enabled_flag,
                        const bool first_detumbl_flag,
                        const bool rotation_ok,
                        bool time_elapsed,
                        bool *set_first_detumbl_flag,
                        bool *set_detumbl_flag_to);

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

bool sat_rotation() {

  bool rotation_ok = false;
  float rotation = 0.0;
  float rotation_threshold = 0.0;

  get_parameter(SAT_ROTATION_THRS_PAR_ID, &rotation_threshold);
  get_parameter(SAT_ROTATION_PAR_ID, &rotation);

  if(rotation < rotation_threshold) {
    rotation_ok = true;
  }

  return rotation_ok;

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
                     const bool activated_flag,
                     const bool time_elapsed,
                     bool *set_activated_flag) {

  bool transition = false;
  *set_activated_flag = false;

  if(activated_flag == true) {

    transition = true;
    *next_mode = DEPLOYMENT_MODE_ID;

  } else if(activated_flag == false && time_elapsed == true) {

    transition = true;
    *set_activated_flag = true;
    *next_mode = DEPLOYMENT_MODE_ID;

  } else if(activated_flag == false && time_elapsed == false) {

    transition = false;
    *next_mode = ACTIVATION_MODE_ID;

  } else {
    //error
  }

  return transition;
}

bool deployment_mode(sat_mode_id *next_mode,
                     const bool deployed_flag,
                     const bool deployed,
                     const bool batt_ok,
                     bool time_elapsed
                     bool *set_deployed_flag) {

  bool transition = false;
  *set_deployed_flag = false;

  if(deployed_flag == true) {

    transition = true;
    *next_mode = SAFE_MODE_ID;

  } else if(deployed_flag == false && deployed == true) {

    transition = true;
    *set_deployed_flag = true;
    *next_mode = SAFE_MODE_ID;

  } else if(deployed_flag == false && batt_ok == true) {

    transition = false;

  } else if(state == WAIT_DEPLOYED_DEP_ID && time_elapsed == false) {

    transition = false;

  } else {
    //error
  }

  return transition;
}

bool safe_mode(sat_mode_id *next_mode, const bool batt_charged) {

  bool transition = false;

  if(batt_charged == true) {

    transition = true;
    *next_mode = ADCS_MODE_ID;

  } else {

    transition = false;

  }

  return transition;
}

bool adcs_mode(sat_mode_id *next_mode,
               const bool adcs_enabled_flag,
               const bool first_detumbl_flag,
               const bool rotation_ok,
               bool time_elapsed,
               bool *set_first_detumbl_flag,
               bool *set_detumbl_flag_to) {

  bool transition = false;
  *set_first_detumbl_flag = false;
  *set_detumbl_flag = false;

  if(adcs_enabled_flag == false) {

    transition = true;
    *next_mode = NOMINAL_MODE_ID;

  } else if(first_detumbl_flag == true) {

    transition = false;
    *set_first_detumbl_flag = true;

  } else if(rotation_ok == true) {

    transition = true;
    *next_mode = NOMINAL_MODE_ID;
    *set_detumbl_flag_to = false;

  } else if(rotation_ok == false && time_elapsed == false) {

    transition = false;
    *set_detumbl_flag_to = true;

  } else if(rotation_ok == false && time_elapsed == true) {

    transition = true;
    *next_mode = SAFE_MODE_ID;
    *set_detumbl_flag_to = false;

  } else {
    //error
  }

  return transition;
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
