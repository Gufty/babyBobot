#pragma once

#include "pros/motors.hpp"
#include "../Constants.h"

using namespace pros;
using namespace Constants;

class Roller {
    private:
        Motor roller = Motor(roller_p);
    public:
        Roller() {
            roller.set_brake_mode(E_MOTOR_BRAKE_HOLD);
        }
        inline void rollerHalfStep(signed char direction = 1) {
            roller.move_relative(200*direction,127);
        }
};