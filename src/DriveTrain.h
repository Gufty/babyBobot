#include "pros/motors.hpp"

using namespace pros;

class DriveTrain {
    Motor fl_mtr = Motor(11);
    Motor bl_mtr = Motor(12);
    Motor fr_mtr = Motor(1);
    Motor br_mtr = Motor(2);

    Motor_Group left_g =  Motor_Group({fl_mtr,bl_mtr});
    Motor_Group right_g = Motor_Group({fr_mtr,br_mtr});

    Motor roller = Motor(5);
    Motor extender = Motor(6);
    int pos;

    public:
        DriveTrain(int team) {
            left_g.set_brake_modes(E_MOTOR_BRAKE_HOLD);
            right_g.set_brake_modes(E_MOTOR_BRAKE_HOLD);

            if (team == 2) {
                right_g.set_reversed(true);
            } else {
                left_g.set_reversed(true);
            }

            pos = extender.get_position();
        }

        void tankDrive(int leftY, int rightY){
            left_g.move(leftY);
            right_g.move(rightY);
        }
        void arcadeDrive(int leftY, int rightX) {
            left_g.move(leftY + rightX);
            right_g.move(leftY - rightX);
        }
        void moveRoller(int voltage) {
            roller.move(voltage);
        }
        void extend(int voltage) {
            extender.move(voltage);
        }
        void stayExtender() {
            extender.move_absolute(pos, 30);
        }
};