#pragma once

#include "pros/motors.hpp"
#include "../Constants.hpp"

using namespace pros;
using namespace Constants;

class Troller {
    private:
        Motor troller = Motor(troller_p);
    public:
        Troller() {
            troller.set_brake_mode(E_MOTOR_BRAKE_HOLD);
        }
        inline void halfStep(signed char direction = 1) {
            troller.move_relative(200*direction,127);
        }
        inline void move(signed char voltage) {
            troller.move(voltage);
        }
};