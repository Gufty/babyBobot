#pragma once

#include "Path.hpp"
#include "main.h"
#include "../Constants.hpp"
#include "../Display.hpp"
#include "../systems/DriveTrain.hpp"
#include "Vector2.hpp"

using namespace Constants;
using namespace Display;
using namespace pros;

#define ODOM_DEBUG

class Odometry {
	private:
		DriveTrain* dt;

		double newLeft, newRight;
		double deltaLeft, deltaRight, deltaCenter;
		double phi;
		double deltaPerp;

		lv_obj_t** odometryInfo;

		Path p;

	public:
		Vector2 pos = {0,0};
		double heading = 0.0;
		double leftEncoder = 0.0;
		double rightEncoder = 0.0;

		std::function<void(void*)> odomTick = [=](void*){
			newLeft = (dt->fl_mtr.get_position() + dt->bl_mtr.get_position()) / 2;
			newRight = (dt->fr_mtr.get_position() + dt->br_mtr.get_position()) / 2;

			deltaLeft = newLeft - leftEncoder;
			deltaRight = newRight - rightEncoder;
			deltaCenter = (deltaLeft + deltaRight) / 2;

			phi = (deltaLeft - deltaRight) / (trackwidth * inchesToUnits);
			deltaPerp = deltaCenter * phi;
			
			pos.x += (deltaCenter * cos(heading) - deltaPerp * sin(heading))/inchesToUnits;
			pos.y += (deltaCenter * sin(heading) + deltaPerp * cos(heading))/inchesToUnits;
			heading += phi;

			leftEncoder = newLeft;
			rightEncoder = newRight;

#ifdef ODOM_DEBUG
			lv_label_set_text(*odometryInfo, ("Position: (" + std::to_string(pos.x) + ", " + std::to_string(pos.y) + ") Theta: " + std::to_string(heading)).c_str());
#endif
			delay(10);
		};
        Odometry(DriveTrain* dt, lv_obj_t** odometryInfo):dt(dt), odometryInfo(odometryInfo){}
		Task odomTask{odomTick};

		inline void loadPath(const char* path2path) {
			unsigned short n = 1;
			FILE* usd_file_read = fopen(path2path, "r");

			if (usd_file_read != NULL)
			{
				printf("File found.");

				short newlines;
				while( (newlines = fgetc(usd_file_read)) != EOF)
				{if (newlines == 10){n++;}}

				rewind(usd_file_read);
				
				double x[n];
				double y[n];

				for (unsigned short i = 0; i < n; i++) {
					fscanf(usd_file_read, "%lf,%lf ", &x[i], &y[i]);
				}
				fclose(usd_file_read);

				Vector2 points[n];

				for(unsigned short i = 0; i < n; i++){
					points[i] = {x[i], y[i]};
				}

				p = Path(points, n);
			}
		}

		inline void followPath(const char* path2path){
			loadPath(path2path);
			for (unsigned short i = 1; i < p.n; i++) {
				dt->turn(p.points[i-1].degreesTo(heading, p.points[i]));
				dt->moveForward(p.points[i-1].distanceTo(p.points[i]));
			}
		} 
};