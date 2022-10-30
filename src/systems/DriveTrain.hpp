#pragma once

#include "../Constants.hpp"
#include "pros/motors.hpp"

using namespace Constants;
using namespace pros;

struct DriveTrain {
  Motor fl_mtr = Motor(fl_p);
  Motor bl_mtr = Motor(bl_p);
  Motor fr_mtr = Motor(fr_p);
  Motor br_mtr = Motor(br_p);

  Motor_Group left_g =  Motor_Group({fl_mtr,bl_mtr});
  Motor_Group right_g = Motor_Group({fr_mtr,br_mtr});

  std::function<void(void)> teleMove;

  DriveTrain() {
    left_g.set_brake_modes(E_MOTOR_BRAKE_HOLD);
    right_g.set_brake_modes(E_MOTOR_BRAKE_HOLD);
    left_g.set_reversed(true);

    left_g.tare_position();
    right_g.tare_position();
  }
  inline void tankDrive(signed char leftY, signed char rightY){
    left_g.move(abs(leftY)<threshold ? 0 :leftY);
    right_g.move(abs(rightY)<threshold ? 0 :rightY);
  }
  inline void arcadeDrive(signed char leftY, signed char rightX) {
    leftY = abs(leftY)<threshold ? 0 : leftY;
    rightX = abs(rightX)<threshold ? 0 : rightX;
    left_g.move(leftY + rightX);
    right_g.move(leftY - rightX);
  }
  inline void moveForward(signed short distance) {
    float ValueLeft = left_g.get_position()[0];
    float ValueRight = right_g.get_position()[0];

    float AverageValueEncoder;

    const double kP = 0.0;
    const double kI = 0.0;
    const double kD = 0.0;

    float integral;
    float derivative;

    float error;
    float prevError;
    float speed;

    while (fabs(error) > 0.1) {
      AverageValueEncoder = (((float)left_g.get_position()[0] - ValueLeft) + ((float)right_g.get_position()[0]) - ValueRight) / 2;

      error = distance - AverageValueEncoder;
      integral += error;

      if (error == 0) {
        integral = 0;
      }
      if (fabs(error) > 40) {
        integral = 0;
      }

      derivative = error - prevError;

      prevError = error;
      speed = (kP * error) + (kI * integral) + (kD * derivative);

      left.move_voltage(speed);
      right.move_voltage(speed);
    }
  }
}
inline void turn(signed short degrees) {
  left_g.move_relative(degrees, 127);
  right_g.move_relative(-degrees, 127);
}
};
