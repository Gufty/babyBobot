#pragma once

#include "pros/motors.hpp"
#include "../Constants.hpp"
#include "main.h"

using namespace Constants;
using namespace pros;

class Extender {
    private:
        Motor xtender = Motor(xtend_p);
    public:
        Extender() {
            xtender.set_brake_mode(E_MOTOR_BRAKE_HOLD);
        }
        void crossXtend(signed char direction = 1) {
            xtender.move_relative(450*direction, 69);
        };
};