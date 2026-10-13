#pragma once

#include <cmath>

namespace fsm {
// Only a completed main trajectory ending at the requested goal can become
// a stationary hold. Expired intermediate/backup trajectories still replan.
inline bool terminalGoalHoldEligible(bool finished, bool main_trajectory,
                                     double endpoint_error, double speed,
                                     double acceleration) {
    return finished && main_trajectory && std::isfinite(endpoint_error)
           && std::isfinite(speed) && std::isfinite(acceleration)
           && endpoint_error >= 0.0 && endpoint_error < 0.1
           && speed >= 0.0 && speed < 1e-3
           && acceleration >= 0.0 && acceleration < 1e-3;
}
}
