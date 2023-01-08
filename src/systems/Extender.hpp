#pragma once

#include "pros/adi.hpp"
#include "../Constants.hpp"
#include "pros/rtos.hpp"

using namespace Constants;
using namespace pros;

class Extender {
    private:
        ADIDigitalOut xtend1 = ADIDigitalOut(xtend1_p, false);
        ADIDigitalOut xtend2 = ADIDigitalOut(xtend2_p, false);
    public:
        Extender() {}
        void set(bool state) {
            xtend1.set_value(state);
            xtend2.set_value(state);
        };
};