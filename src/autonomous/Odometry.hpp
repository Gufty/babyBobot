#pragma once

#include "main.h"
#include "../Constants.hpp"
#include "../Display.hpp"
#include "../systems/DriveTrain.hpp"
#include "Vector2.hpp"
#include "Path.hpp"
#include "pros/rtos.hpp"
#include <cstdio>
#include <sys/_intsup.h>

using namespace Constants;
using namespace Display;
using namespace pros;

#define ODOM_DEBUG

class Odometry {
	private:
		DriveTrain* dt;
		lv_obj_t** odometryInfo;
		
		Vector2 points[2] = {{0,48},{48,48}};
		double distances[1] = {24};
		Path p = Path(points,distances,2);
		
		double newLeft, newRight;
		double phi;
		double dLeft, dRight, rCenter;
		double hCos, hSin, pCos, pSin;

	public:
		Vector2 pos = {0,0};
		double heading = pi/2;
		double leftEncoder, rightEncoder;

		void odomTick(){
			while(true) {
			newLeft = (dt->fl_mtr.get_position() + dt->bl_mtr.get_position()) / 2 * inchesPerTick;
			newRight = (dt->fr_mtr.get_position() + dt->br_mtr.get_position()) / 2 * inchesPerTick;
			
			dLeft = newLeft - leftEncoder;
			dRight = newRight - rightEncoder;

			phi = (dRight-dLeft)/trackwidth;

			if (phi == 0) {
				pos.x += dLeft*cos(heading);
				pos.y += dLeft*sin(heading);
			} else {
				rCenter = ((dLeft+dRight)/2)/phi;

				hCos = cos(heading); hSin = sin(heading);
				pCos = cos(phi); pSin = sin(phi);

				pos.x += (rCenter)*(-hSin + pSin*hCos + hSin*pCos);
				pos.y += (rCenter)*(hCos - pCos*hCos + hSin*pSin);
			}
			heading+=phi;

			heading = headingRestrict(heading);

			//std::cout << pos.x << " " << pos.y << " " << heading*radToDeg << " " << phi*radToDeg << std::endl;

			leftEncoder = newLeft;
			rightEncoder = newRight;

#ifdef ODOM_DEBUG
			lv_label_set_text(*odometryInfo, ("Position: (" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ") \nTheta: " + std::to_string(heading*radToDeg)).c_str());
#endif
			delay(10);
			}
		};

        Odometry(DriveTrain* dt, lv_obj_t** odometryInfo):dt(dt), odometryInfo(odometryInfo){}
		Task odomTask{std::bind(&Odometry::odomTick, this)};

		inline void loadPath(const char* path2points, const char* path2dists) {
			unsigned short n = 1;
			FILE* points_read = fopen(path2points, "r");

			if (points_read == NULL)
			{
				printf("File not found.");
				throw "NERD";
			}
			short newlines;
			while( (newlines = fgetc(points_read)) != EOF)
			{if (newlines == 10){n++;}}

			rewind(points_read);
			
			Vector2 points[n];

			for (unsigned short i = 0; i < n; i++) {
				fscanf(points_read, "%lf,%lf ", &points[i].x, &points[i].y);
			}
			fclose(points_read);

			FILE* dists_read = fopen(path2dists, "r");

			double arcDists[n-1];
			for (unsigned short i = 0; i < n-1; i++) {
				fscanf(dists_read, "%lf", &arcDists[i]);
			}

			fclose(dists_read);

			p = Path(points, arcDists, n);
		}
		inline void moveDist(double distanceL, double distanceR) {
			float ValueLeft = leftEncoder;
			float ValueRight = rightEncoder;

			float deltaLeft, deltaRight;

			const double kP = 8.7;
			const double kI = 0.0;
			const double kD = 0.0;

			float lIntegral, lPrevIntegral;
			float lDerivative;
			float lError, lPrevError;
			float lSpeed;

			float rIntegral, rPrevIntegral;
			float rDerivative;
			float rError, rPrevError;
			float rSpeed;

			unsigned char iterationTime = 20;

			while (true) {
				deltaLeft = leftEncoder - ValueLeft;
				deltaRight = rightEncoder - ValueRight;

				lError = distanceL - deltaLeft;
				rError = distanceR - deltaRight;

				if(fabs(lError) < 0.1 && fabs(rError) < 0.1){break;}

				lIntegral = lPrevIntegral + lError * iterationTime;
				rIntegral = rPrevIntegral + rError * iterationTime;

				lDerivative = (lError - lPrevError)/iterationTime;
				rDerivative = (rError - rPrevError)/iterationTime;

				lSpeed = (kP * lError) + (kI * lIntegral) + (kD * lDerivative);
				rSpeed = (kP * rError) + (kI * rIntegral) + (kD * rDerivative);

				dt->left_g.move(lSpeed);
				dt->right_g.move(rSpeed);

				lPrevError = lError;
				rPrevError = rError;

				lPrevIntegral = lIntegral;
				rPrevIntegral = rIntegral;

				delay(iterationTime);
			}
			float InitialHeading = heading;
		}
		inline void followPath(){
			float time = 0.0;
			Vector2 target;

			double pathDist;

			double distance;
			double deltaTime;
			double direction;
			while (true) {
				pathDist = p.timeToDis(time, 0.1, -0.1); 
				if (pathDist>=p.totalDistance){break;}
				target = p.distToPos(pathDist);

				distance = trackwidth*pos.headingTo(heading, target)/pi;
				deltaTime = fabs(distance)/maxActVel;
				direction = (distance<0) ? -1 : 1;

				std::cout << distance << " " << deltaTime << std::endl;

				dt->left_g.move_velocity(direction*-maxIdealVel);
				dt->right_g.move_velocity(direction*maxIdealVel);

				time += deltaTime;
				delay(deltaTime/10);

				distance = pos.distanceTo(target);
				deltaTime = fabs(distance)/maxActVel;
				direction = (distance<0) ? -1 : 1;

				std::cout << distance << " " << deltaTime << std::endl;

				dt->left_g.move_velocity(direction*maxIdealVel);
				dt->right_g.move_velocity(direction*maxIdealVel);
				
				time += deltaTime;
				delay(deltaTime/10);
			}
			dt->tankDrive(0,0);
		}
};