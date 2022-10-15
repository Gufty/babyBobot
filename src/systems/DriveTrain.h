#pragma once

#include "pros/motors.hpp"
using namespace pros;

extern int fl_p;
extern int bl_p;
extern int fr_p;
extern int br_p;
extern int threshold;
extern int inchesToUnits;

class DriveTrain {
    Motor fl_mtr = Motor(fl_p);
    Motor bl_mtr = Motor(bl_p);
    Motor fr_mtr = Motor(fr_p);
    Motor br_mtr = Motor(br_p);

    Motor_Group left_g =  Motor_Group({fl_mtr,bl_mtr});
    Motor_Group right_g = Motor_Group({fr_mtr,br_mtr});

    public:
        DriveTrain() {
            left_g.set_brake_modes(E_MOTOR_BRAKE_HOLD);
            right_g.set_brake_modes(E_MOTOR_BRAKE_HOLD);
            right_g.set_reversed(true);
        }

        void tankDrive(int leftY, int rightY){
            left_g.move(abs(leftY)<threshold ? 0 :leftY);
            right_g.move(abs(rightY)<threshold ? 0 :rightY);
        }
        void arcadeDrive(int leftY, int rightX) {
            leftY = abs(leftY)<threshold ? 0 : leftY;
            rightX = abs(rightX)<threshold ? 0 : rightX;
            left_g.move(leftY + rightX);
            right_g.move(leftY - rightX);
        }
        void moveForward(int inches) {
            left_g.move_relative(inches*inchesToUnits , 127);
        }
};