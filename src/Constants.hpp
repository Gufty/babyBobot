#pragma once

namespace Constants {
    static constexpr unsigned char fl_p = 11;
    static constexpr unsigned char bl_p = 12;
    static constexpr bool fr_p = 1;
    static constexpr unsigned char br_p = 2;

    static constexpr unsigned char roller_p = 5;
    static constexpr unsigned char xtend_p = 6;

    static constexpr unsigned char threshold = 10;

    static constexpr float robotLength = 18.0f;

    static constexpr float wheelDiameter = 4.0f;
    static constexpr float trackwidth = 15.0f;

    static constexpr unsigned short unitstoRevolution = 360;
    static constexpr unsigned char inchesToUnits = 2;

    static constexpr inline float f_sqrt(float n) {
      float x = n;
      float y = 1;
      float e = 0.0001; // e decides the accuracy level
      while (x - y > e) {
        x = (x + y) / 2;
        y = n / x;
      }
      return x;
    }

    static constexpr inline float f_atan(float n) {
      return n*(45-15.66*(n-1)); // +- 0.22 degrees
    }
};