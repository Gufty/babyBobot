#pragma once

#include "Vector2.hpp"
#include <cstdlib>
#include <cmath>

class Path {
    public:
        Vector2* points;
        unsigned short n;

        Path(){}
        Path(Vector2* points, unsigned short n): points(points), n(n) {}

        inline float calculateUnmodifiedDistance() {
            float distance = 0.0;
            for (unsigned short i = 1; i < n; i++) {
                distance += points[i-1].distanceTo(points[i]);
            }
            return distance;
        }

        inline float calculateUnmodifiedTime() {
            float maxForwVel = 3;
            float maxTurnVel = 2;
            //time to go from one point to another
            float time = calculateUnmodifiedDistance()/maxForwVel;
            for (unsigned short i = 2; i < n; i++) {
                //calculate time for each turn
                float p12 = points[i-2].distanceTo(points[i-1]);
                float p23 = points[i-1].distanceTo(points[i]);
                float p13 = points[i-2].distanceTo(points[i]);
                //law of cosines baby
                float phi = std::acos((p12*p12 + p23*p23 - p13*p13) / (2*p12*p23));
                time += phi/maxTurnVel;
            }
            return time;
        }

        inline float* arcDistancesFromTime(float targetTime) {
            float timeToCut = calculateUnmodifiedTime()-targetTime;
            float* distances = (float*)malloc(sizeof(float)*n);

            if (timeToCut < 0) {
                for (unsigned short i = 0; i < n; i++) {
                    distances[i] = 0;
                }
                return distances;
            }

            //math needed
            return distances;
        }
};
