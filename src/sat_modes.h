#ifndef _SAT_MODES_H
#define _SAT_MODES_H

#include <stdbool.h>

typedef enum {
    DEBUG_MODE_ID        = 0,
    ACTIVATION_MODE_ID   = 1,
    DEPLOYMENT_MODE_ID   = 2,
    SAFE_MODE_ID         = 3,
    ADCS_MODE_ID         = 4,
    NOMINAL_MODE_ID      = 5,
    OFF_MODE_ID          = 6,
    LAST_MODE_ID         = 7,
    JOHN_MODE_ID         = 8
}sat_mode_id;

typedef enum {
    ACTIVATED_ACT_ID      = 0,
    NOT_ACTIVATED_ACT_ID  = 1,
    LAST_ACT_ID           = 2
}sat_submode_act_id;

struct _sat_state {
    sat_mode_id current_mode;
    sat_mode_id previous_mode;
};

void satellite_mode();

sat_mode_id get_satellite_mode();

bool transition_check_sat_modes(sat_mode_id current, sat_mode_id next);

bool activation_mode(sat_mode_id *next_mode);

#endif // _SAT_MODES_H
