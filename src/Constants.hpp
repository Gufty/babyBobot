#pragma once

#include <cmath>
namespace Constants {
    static constexpr float pi = 3.14159265359; // pi
    static const float radToDeg = 180/pi;

    static constexpr unsigned char fl_p = 11; // Port of Front left motor
    static constexpr unsigned char bl_p = 12; // Port of Back left motor
    static constexpr bool fr_p = 1; // Port of Front right motor
    static constexpr unsigned char br_p = 2; // Port of Back right motor

    static constexpr unsigned char roller_p = 5; // Port of Roller
    static constexpr unsigned char xtend_p = 6; // Port of Extender

    static constexpr unsigned char threshold = 10; // Threshhold for controller to start moving the robot
    
    static constexpr float trackwidth = 11.0f; // The distance between both the left and an right motors

    static constexpr float wheelDiameter = 4.0f; // Diameter of drive wheels
    static constexpr unsigned short unitstoRevolution = 360; // Motor units per revolution for drive wheels
    static const float inchesPerTick = wheelDiameter*pi/unitstoRevolution; // The inches pper tick for the drive encoders

};