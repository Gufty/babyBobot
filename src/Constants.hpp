#pragma once

#include <cmath>
#include <sys/_types.h>
namespace Constants {
    static constexpr double pi = 3.14159265359; // pi
    static const double radToDeg = 180/pi;

    static constexpr unsigned char fl_p = 1; // Port of Front Left motor
    static constexpr unsigned char bl_p = 2; // Port of Back Left motor
    static constexpr unsigned char btl_p = 3; // Port of Top Back Left motor
    static constexpr unsigned char fr_p = 12; // Port of Front Right motor
    static constexpr unsigned char btr_p = 11; // Port of Top Back Right motor
    static constexpr unsigned char br_p = 10; // Port of Back Right motor

    static constexpr unsigned char troller_p = 8; // Port of Roller
    
    static constexpr unsigned char cata_p = 7; // Port of the Catapult

    static constexpr unsigned char button_p = 'A'; // Port of Button for Catapult
    static constexpr unsigned char xtend1_p = 'B'; // Port of the First pneumatics system
    static constexpr unsigned char xtend2_p = 'C'; // Port of the Second pnewumatics system

    static constexpr unsigned char threshold = 11; // Threshhold for controller to start moving the robot
    
    static constexpr float trackwidth = 9.9f; // The distance between both the left and an right motors

    static constexpr float wheelDiameter = 2.25f; // Diameter of drive wheels
    static constexpr double unitstoRevolution = 642.8571429; // Motor units per revolution for drive wheels (900 * 5/7)
    static const float inchesPerTick = wheelDiameter*pi/unitstoRevolution; // The inches per tick for the drive encoders

    static constexpr float maxActVel = 54.0f;
    static const float maxRelVel = maxActVel/inchesPerTick;
    
    static const double accel = 1;
    static constexpr double spacing = 12;

    static inline double headingRestrict(double heading) {
        if (heading <= 0) heading += pi*2;
        heading = fmod(std::abs(heading),pi*2);
        return heading;
    }

    static inline double constrain(double input, double lowerBound, double upperBound) {
        if (input > lowerBound) {
            if (input < upperBound) {
                return input;
            } else {
                return upperBound;
            }
        } else {
            return lowerBound;
        }
    }

    static inline signed char signum(double x){
      return (x > 0) - (x < 0);
    }
};