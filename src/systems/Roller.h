#pragma once

#include "pros/motors.hpp"

using namespace pros;

extern int roller_p;

class Roller {
    private:
        Motor roller = Motor(roller_p);
    public:
        void rollerHalfStep(int direction = 1) {
            roller.move_relative(45,127);
        }
};