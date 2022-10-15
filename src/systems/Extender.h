#pragma once

#include "pros/motors.hpp"

using namespace pros;

extern int xtend_p;

class Extender {
    private:
        Motor xtender = Motor(xtend_p);
    public:
        Task stay{[=]{
            xtender.move_relative(-xtender.get_position(), 30);
            delay(20);
        }};

        std::function<void(int)> spoolXtend;
        std::function<void(int)> crossXtend;

        Extender() {
            xtender.set_brake_mode(E_MOTOR_BRAKE_HOLD);
            xtender.tare_position();

            //needlessly complicated way to remove stay.remove()
            spoolXtend = [=](int direction){
                stay.remove();
                xtender.move_relative(45*direction, 69);

                spoolXtend = [=](int direction){
                xtender.move_relative(45*direction, 69);
                };
            };

            crossXtend = [=](int direction){
                stay.remove();
                xtender.move_relative(80*direction, 69);

                crossXtend = [=](int direction){
                xtender.move_relative(80*direction, 69);
                };
            };
        }


};