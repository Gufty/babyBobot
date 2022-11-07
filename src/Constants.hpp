#pragma once

#include <cmath>
namespace Constants {
    static constexpr double pi = 3.14159265359; // pi
    static const double radToDeg = 180/pi;

    static constexpr unsigned char fl_p = 11; // Port of Front left motor
    static constexpr unsigned char bl_p = 12; // Port of Back left motor
    static constexpr bool fr_p = 1; // Port of Front right motor
    static constexpr unsigned char br_p = 2; // Port of Back right motor

    static constexpr unsigned char roller_p = 5; // Port of Roller
    static constexpr unsigned char xtend_p = 8; // Port of Extender

    static constexpr unsigned char threshold = 10; // Threshhold for controller to start moving the robot
    
    static constexpr float trackwidth = 9.9f; // The distance between both the left and an right motors

    static constexpr float wheelDiameter = 4.0f; // Diameter of drive wheels
    static constexpr double unitstoRevolution = 642.8571429; // Motor units per revolution for drive wheels (900 * 5/7)
    static const float inchesPerTick = wheelDiameter*pi/unitstoRevolution; // The inches pper tick for the drive encoders

    static constexpr float maxActVel = 54.0f;
    static const float maxIdealVel = 58.8/inchesPerTick;

    static inline double headingRestrict(double heading) {
        if (heading <= 0) heading += pi*2;
        heading = fmod(std::abs(heading),pi*2);
        return heading;
    }
};