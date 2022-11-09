#pragma once

#include "Vector2.hpp"
#include "../Constants.hpp"
#include "Arc.hpp"
#include "Line.hpp"
#include <cmath>
#include <cstdlib>
using namespace Constants;

class Path {
    public:
        Vector2* pPoints;
        double* arcDists;
        unsigned short n;

        double* angles;
        double* distances;
        double* arcLengths;
        double totalDistance;

        Line* lines;
        Arc* arcs;
    private:
        inline void returnAngles() {
            float phi, theta = 0;
            angles = (double*)malloc(sizeof(double)*(n-1));
            for(unsigned short i = 0; i < n-1; i++) {
                phi = pPoints[i].headingTo(theta, pPoints[i+1]);
                angles[i] = headingRestrict(phi);
                theta += phi;
            }
            angles[n-1]=pi/2; 
        }
        inline void createPath() {
            lines = (Line*)malloc(sizeof(Line)*(n-1));
            arcs = (Arc*)malloc(sizeof(Arc)*(n-2));
            Vector2 temp1; Vector2 temp2;
            for (unsigned short i = 0; i < n-1; i+=2) {
                unsigned short v = i/2;
                unsigned short h = v+1;

                temp1.x = pPoints[h].x - cos(angles[v])*arcDists[v];
                temp1.y = pPoints[h].y - sin(angles[v])*arcDists[v];

                temp2.x = pPoints[h].x + cos(angles[h])*arcDists[v];
                temp2.y = pPoints[h].y + sin(angles[h])*arcDists[v];

                lines[v+v] = Line(pPoints[v], temp1);
                lines[h+v] = Line(temp2, pPoints[v+2]);

                arcs[v] = Arc(temp1, pPoints[h], temp2);
            }
        }
        inline void returnDistances(){
            distances = (double*)malloc(sizeof(double)*(n-1));
            for (unsigned short i = 0; i < n-1; i++) {
                distances[i]=lines[i].disBtwnCords;
            }
        } 
        inline void returnArcLengths(){
            arcLengths = (double*)malloc(sizeof(double)*(n-2));
            for (unsigned short i = 0; i < n-2; i++) {
                arcLengths[i] = arcs[i].arcL;
            }
        }
        inline void calculateTotalDistance() {
            for(unsigned short i = 0; i < n-1; i++){
                totalDistance += distances[i];
            }
            for(unsigned short i = 0; i < n-2; i++){
                totalDistance += arcLengths[i];
            }
        }
        /*
        inline float calculateUnmodifiedTime() {
            //time to go from one point to another
            return totalDistance/maxForwVel + totalAngles/maxTurnVel;
        }
        inline Vector2* arcDistancesFromTime(float targetTime) {
            float timeToCut = targetTime-calculateUnmodifiedTime();
            Vector2* arcDists = (Vector2*)malloc(sizeof(Vector2)*n-2);

            if (timeToCut < 0) {
                for (unsigned short i; i < n-1; i++) {
                    arcDists[i] = {0,0};
                }
                return arcDists;
            }

            unsigned int mod;
            double absPhi;
            float temp = timeToCut;

            while(temp<0) {
                temp=timeToCut;
                mod++;
                for(unsigned short i = 0; i < n-1; i++) {
                    absPhi = fmod(fabs((angles[i]<=0)?angles[i]+pi*2:angles[i]),pi*2);
                    if (angles[i]<0){
                        arcDists[i].x=(mod+trackwidth/2)*absPhi;
                        arcDists[i].y=(mod-trackwidth/2)*absPhi;
                    } else {
                        arcDists[i].y=(mod+trackwidth/2)*absPhi;
                        arcDists[i].x=(mod-trackwidth/2)*absPhi;
                    }
                    temp-=arcDists[i].x/maxForwVel;
                }
            }
            return arcDists;
        }*/
    public:
        Path(Vector2* pPoints, double* arcDists, unsigned short n): pPoints(pPoints), arcDists(arcDists), n(n) {
            returnAngles();
            createPath();
            returnDistances();
            returnArcLengths();
            calculateTotalDistance();
        }
        inline double timeToDis(double t, double a1, double a2) {
            double dt1 = maxActVel / a1;
            double dt3 = fabs(maxActVel / a2);
            double dd1 = maxActVel * dt1 * .5;
            double dd3 = maxActVel * dt3 * .5;
            double dd2 = totalDistance - dd1 - dd3;
            double dt2 = dd2/maxActVel;
            
            if (t >= dt3 + dt2 + dt1)
            return totalDistance;
            else if (t > dt2 + dt1){
            double time = t - dt2 - dt1;
            return dd1 + dd2 + (.5*time*((2*maxActVel) +(a2 *time)));
            }
            else if (t == dt2 + dt1)
            return dd1 + dd2;
            else if (t > dt1)
            return dd1 + (maxActVel * (t - dt1));
            else if (t == dt1)
            return dd1;
            return .5 * a1 * pow(t, 2);
        }
        inline Vector2 distToPos(double distance) {
            Vector2 posAtTime;
            for (unsigned short i = 1; i <= 2*n; i++){
                unsigned short v = i-1;
                if (i%2==0) {
                    if (distance > arcs[v].arcL){
                        distance -= arcs[v].arcL;
                    } else {
                        posAtTime = arcs[v].distanceToCoordinate(distance);
                        break;
                    }
                } else {
                    if (distance > distances[v]) {
                        distance -= distances[v];
                    } else {
                        posAtTime = lines[v].distanceToCoordinate(distance);
                        break;
                    }
                }
            }
            return posAtTime;
        }
};
