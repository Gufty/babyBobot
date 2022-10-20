#pragma once

#include "pros/motors.hpp"
#include "../Constants.h"
using namespace Constants;
using namespace pros;

class Extender {
    private:
        Motor xtender = Motor(xtend_p);
    public:
        Extender() {
            xtender.set_brake_mode(E_MOTOR_BRAKE_HOLD);
        }
        void spoolXtend(signed char direction = 1) {
            xtender.move_relative(45*direction, 69);
        };
        void crossXtend(signed char direction = 1) {
            xtender.move_relative(80*direction, 69);
        };
};