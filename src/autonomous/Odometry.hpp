#pragma once

#include "main.h"
#include "../Constants.hpp"
#include "../Display.hpp"
#include "../systems/DriveTrain.hpp"
#include "Vector2.hpp"
#include "Path.hpp"

using namespace Constants;
using namespace Display;
using namespace pros;

#define ODOM_DEBUG

class Odometry {
	private:
		DriveTrain* dt;
		lv_obj_t** odometryInfo;
		
		Path p;
		
		double newLeft, newRight;
		double phi;
		double dLeft, dRight, rCenter;
		double hCos, hSin, pCos, pSin;

	public:
		Vector2 pos = {0,0};
		double heading = pi/4;
		double leftEncoder, rightEncoder;

		void odomTick(){
			while(true) {
			newLeft = ((*dt).fl_mtr.get_position() + (*dt).bl_mtr.get_position()) / 2 * inchesPerTick;
			newRight = ((*dt).fr_mtr.get_position() + (*dt).br_mtr.get_position()) / 2 * inchesPerTick;
			
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

			if (heading <= 0) heading += pi*2;
			heading = fmod(std::abs(heading),pi*2);

			std::cout << pos.x << " " << pos.y << " " << heading*radToDeg << " " << phi*radToDeg << std::endl;

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

		inline void loadPath(const char* path2path) {
			unsigned short n = 1;
			FILE* usd_file_read = fopen(path2path, "r");

			if (usd_file_read == NULL)
			{
				printf("File not found.");
				throw "NERD";
			}
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

		inline void followPath(){
			(*dt).turn(p.angles[0]-heading);
			(*dt).moveForward(pos.distanceTo(p.points[0]));

			for (unsigned short i = 1; i < p.n-1; i++) {
				(*dt).turn(p.angles[i]);
				(*dt).moveForward(p.distances[i-1]);
			}
		}
};