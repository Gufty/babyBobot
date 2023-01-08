#pragma once

#include "../Constants.hpp"
#include "pros/adi.hpp"
#include "pros/motors.hpp"

using namespace Constants;
using namespace pros;

class Catapult {
    private:
        Motor cata = Motor(cata_p);
        ADIDigitalIn button = ADIDigitalIn(button_p);
    public:
        Catapult(){}
        void move(signed char voltage, bool restricted) {
            if (voltage > 0 && restricted && button.get_value()) {return;}
            cata.move(voltage);
        }
};