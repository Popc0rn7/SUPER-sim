#include "fsm/terminal_hold.hpp"
#include <cassert>
#include <limits>

int main() {
    using fsm::terminalGoalHoldEligible;
    // A valid terminal target remains available while measured tracking error
    // settles; measured vehicle position is deliberately not this predicate.
    assert(terminalGoalHoldEligible(true, true, 1e-6, 1e-7, 1e-7));
    assert(!terminalGoalHoldEligible(false, true, 0, 0, 0));
    assert(!terminalGoalHoldEligible(true, false, 0, 0, 0));
    assert(!terminalGoalHoldEligible(true, true, 2.0, 0, 0));
    assert(!terminalGoalHoldEligible(true, true, 0.1, 0, 0));
    assert(!terminalGoalHoldEligible(true, true, 0, 0.3, 0));
    assert(!terminalGoalHoldEligible(true, true, 0, 0, 0.1));
    const auto nan = std::numeric_limits<double>::quiet_NaN();
    assert(!terminalGoalHoldEligible(true, true, nan, 0, 0));
    assert(!terminalGoalHoldEligible(true, true, 0, nan, 0));
    assert(!terminalGoalHoldEligible(true, true, 0, 0, nan));
}
