#pragma once

#include "main.h"
#include <iostream>
#include "Vector2.hpp"
#include "../Constants.hpp"
using namespace Constants;
#include <cmath>
 
class Arc{
    public:
    float radius;
    float dtheta;
    float arcL;
    float rtheta1;
    float rtheta2;
    Vector2 center;
    Vector2 I1;
    Vector2 I2;
    Arc(Vector2 I1, Vector2 P, Vector2 I2):I1(I1),I2(I2)
    {
        float d1 = I1.distanceTo(P);
        float d2 = I1.distanceTo(I2);
        dtheta = 2 * acosf((d2 / 2) / d1);
        radius = (d2 / 2) / sinf(dtheta / 2);
        arcL = radius * dtheta;
        rtheta1 = gATan(I1.y - P.y, I1.x - P.x);
        //cout << "t1 dy " <<  I1.y - P.y << endl;
        //cout << "t1 dx" << I1.x - P.x << endl;
        //cout << "theta1 " << rtheta1 * 180 / pi << endl;
        rtheta2 = gATan(I2.y - P.y,  I2.x - P.x);
        //cout << "t2 dy " <<  I2.y - P.y << endl;
        //cout << "t2 dx" << I2.x - P.x << endl;
        //cout << "theta2 " << rtheta2 * 180 / pi << endl;
        float thetaToCenter = (rtheta1 + rtheta2) / 2;
        //cout << "PtoCenter angle " << thetaToCenter * 180 / pi << endl;
        float PtoCenter = (2 * radius * d1) / d2;
        //cout << "PtoCenter " << PtoCenter << endl;
        center = {P.x + PtoCenter * cos(thetaToCenter), P.y + PtoCenter * sin(thetaToCenter)};
        rtheta1 = gATan((I1.y - center.y), (I1.x - center.x));
        //cout << "theta1 " << rtheta1 * 180 / pi << endl;
        rtheta2 = gATan((I2.y - center.y), (I2.x - center.x));
        //cout << "theta2 " << rtheta2 * 180 / pi << endl;
    };

    Vector2 distanceToCoordinate(float distance)
    {
        float ntheta = distance / radius;
        Vector2 point;
        if(ntheta > dtheta)
        {
            return I2;
        }
        if(rtheta2 > rtheta1)
        {
            point = {center.x + cos(rtheta1 + ntheta) * radius, center.y + sin(rtheta1 + ntheta) * radius};
        }
        else {
            point = {center.x + cos(rtheta1 - ntheta) * radius, center.y + sin(rtheta1 - ntheta) * radius};
        }
        //cout << "nTheta" << ntheta * 180 / pi << endl;
        return point;
    };

    float gATan(float y, float x)
    {
        float theta = atanf(y / x);
        if(theta < 0)
        {
            theta += 2 * pi;
        }
        if(y > 0 && x < 0)
        {
            theta -= pi;
        }
        else if(y < 0 && x < 0)
        {
            theta += pi;
        }
        return theta;
    }
};