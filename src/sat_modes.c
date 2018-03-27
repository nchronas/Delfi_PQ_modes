#include "sat_modes.h"
#include "satellite.h"

static struct _sat_state sat_state = {
  .current_mode = ACTIVATION_MODE_ID,
  .previous_mode = ACTIVATION_MODE_ID
};

void satellite_mode() {

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
