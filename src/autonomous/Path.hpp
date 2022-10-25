#pragma once

#include "Vector2.hpp"
#include <cmath>
#include <cstdlib>

class Path {
    private:
        inline void returnDistances(){
            distances = (float*)malloc(sizeof(float)*n-1);
            for (unsigned short i = 1; i < n; i++) {
                distances[i-1] = points[i-1].distanceTo(points[i]);
            }
        }
        inline void returnAngles() {
            float phi, theta = 0;
            for(unsigned short i = 1; i < n; i++) { 
                phi = points[i-1].degreesTo(theta, points[i]);
                angles[i-1] = phi;
                theta += phi;
            }
        }
        inline void calculateTotalDistance() {
            for(unsigned short i = 0; i < n-1; i++){
                totalDistance += distances[i];
            }
        }
        inline void calculateTotalAbsAngles() {
            for (unsigned short i = 1; i < n-1; i++){
                totalAngles += std::abs(angles[i]);
            }
        }
        inline float calculateUnmodifiedTime() {
            float maxForwVel = 1;
            float maxTurnVel = 1;
            //time to go from one point to another
            return totalDistance/maxForwVel + totalAngles/maxTurnVel;
        }
        inline float* arcDistancesFromTime(float targetTime) {
            float timeToCut = calculateUnmodifiedTime()-targetTime;
            float* arcDists = (float*)malloc(sizeof(float)*n-2);

            if (timeToCut < 0) {
                for (unsigned short i; i < n-1; i++) {
                    distances[i] = 0;
                }
                return distances;
            }

            //math needed
            return distances;
        }
    public:
        Vector2* points;
        unsigned short n;
        float* distances;
        float* angles;
        float totalDistance;
        float totalAngles;

        Path(){}
        Path(Vector2* points, unsigned short n): points(points), n(n) {
            returnDistances();
            returnAngles();
            calculateTotalDistance();
            calculateTotalAbsAngles();
        }
};
