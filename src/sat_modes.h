#ifndef _SAT_MODES_H
#define _SAT_MODES_H

#include <stdbool.h>

typedef enum {
    ACTIVATION_MODE_ID   = 0,
    DEPLOYMENT_MODE_ID   = 1,
    SAFE_MODE_ID         = 2,
    ADCS_MODE_ID         = 3,
    NOMINAL_MODE_ID      = 4,
    OFF_MODE_ID          = 5,
    LAST_MODE_ID         = 6,
    JOHN_MODE_ID         = 7
}sat_mode_id;

struct _sat_state {
    sat_mode_id current_mode;
    sat_mode_id previous_mode;
};

void satellite_mode();

sat_mode_id get_satellite_mode();

bool transition_check_sat_modes(sat_mode_id current, sat_mode_id next);

#endif // _SAT_MODES_H
